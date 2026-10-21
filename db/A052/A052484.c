/*
 * A052484  Let P = all numbers that can be obtained by permuting the digits
 *          of n and possibly adding or omitting zeros; a(n) = |n-q| where q
 *          in P is the closest number to n different from n (a(n)=0 if no
 *          such q exists).
 * https://oeis.org/A052484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = |n - q| where q ∈ P is the closest number to n different
 *            from n, P = all numbers obtainable by permuting the digits of n
 *            and possibly adding or omitting zeros
 * Range:     u8 n <= 17, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The algorithm enumerates all permutations of the digits of n
   (with zero or one additional zero) because adding more zeros only
   increases the difference. */
#include <stdint.h>
#include <stdbool.h>

/* Standard next_permutation implementation for an int array. */
static bool next_permutation(int *a, int *end)
{
    if (a == end || a + 1 == end)
        return false;
    /* Step 1: find the rightmost index i such that a[i] < a[i+1] */
    int *i = end - 2;
    while (i >= a && *i >= *(i + 1))
        --i;
    if (i < a) {                     /* already the last permutation */
        int *p = a, *q = end - 1;
        while (p < q) {              /* reverse whole array */
            int t = *p; *p = *q; *q = t;
            ++p; --q;
        }
        return false;
    }
    /* Step 2: find rightmost j > i such that a[j] > a[i] */
    int *j = end - 1;
    while (*j <= *i)
        --j;
    /* Step 3: swap a[i] and a[j] */
    int tmp = *i; *i = *j; *j = tmp;
    /* Step 4: reverse the subsequence a[i+1] ... a[end-1] */
    int *p = i + 1, *q = end - 1;
    while (p < q) {
        int t = *p; *p = *q; *q = t;
        ++p; --q;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Handle n = 0 directly */
    if (n == 0)
        return 0;

    /* Collect decimal digits of n (least significant first) */
    uint64_t tmp = (uint64_t)n;
    int digits[20];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    }

    /* Separate non‑zero digits and count zeros */
    int nonzero[20];
    int nz = 0, zeros = 0;
    for (int i = 0; i < len; ++i) {
        if (digits[i] == 0)
            ++zeros;
        else
            nonzero[nz++] = digits[i];
    }

    seq_t n_val = (seq_t)n;
    seq_t best = 0;   /* minimal positive difference found */
    bool found = false;

    /* Enumerate: omit 0..zeros zeros, and add 0 or 1 extra zero */
    for (int omit = 0; omit <= zeros; ++omit) {
        int kept_zeros = zeros - omit;
        for (int add = 0; add <= 1; ++add) {
            int total_zeros = kept_zeros + add;
            int L = nz + total_zeros;   /* length of permutation */

            /* Build array to permute: non‑zero digits + zeros */
            int perm[20];
            for (int i = 0; i < nz; ++i)
                perm[i] = nonzero[i];
            for (int i = 0; i < total_zeros; ++i)
                perm[nz + i] = 0;

            /* Sort to start with the lowest lexicographic permutation */
            for (int i = 0; i < L - 1; ++i)
                for (int j = i + 1; j < L; ++j)
                    if (perm[i] > perm[j]) {
                        int t = perm[i];
                        perm[i] = perm[j];
                        perm[j] = t;
                    }

            /* Iterate over all distinct permutations */
            do {
                /* Compute the integer value of the permutation */
                seq_t val = 0;
                for (int i = 0; i < L; ++i)
                    val = val * 10 + (seq_t)perm[i];

                if (val == n_val)        /* skip the original number */
                    continue;

                seq_t diff = (val > n_val) ? (val - n_val) : (n_val - val);
                if (!found || diff < best) {
                    best = diff;
                    found = true;
                    if (best == 0)
                        return 0;       /* shouldn't happen except n=0 */
                }
            } while (next_permutation(perm, perm + L));
        }
    }

    return found ? best : 0;
}
