/*
 * A216669  Total number of parts in all partitions of n into 2 sizes of parts
 *          (A002133).
 * https://oeis.org/A216669
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n-1} k * A216665(n,k)
 * Range:     u8 n <= 13, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the total number of parts in all partitions of n into exactly two
   distinct part sizes.  The formula derived below counts, for every unordered
   pair of part sizes (a,b), all solutions i*a + j*b = n with i,j≥1 and
   then sums the parts i+j for each solution.  The arithmetic is performed
   with seq_t to avoid overflow for large results. */
#include <stdint.h>

static inline idx_t ugcd(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static idx_t modinv(idx_t a, idx_t mod) {
    /* Extended Euclidean algorithm – all arithmetic in signed 64‑bit */
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt;
        t = newt; newt = tmp;
        tmp = r - q * newr;
        r = newr; newr = tmp;
    }
    if (r <= 1) {            /* gcd is 1 – inverse exists */
        if (t < 0) t += mod;
        return (idx_t)t;
    }
    return 0;  /* this case never occurs for coprime inputs */
}

seq_t a(idx_t n) {
    if (n < 3) return 0;   /* values for n < 3 are not defined in the OEIS series */

    seq_t total = 0;

    for (idx_t a_part = 1; a_part <= n - 2; ++a_part) {
        for (idx_t b_part = a_part + 1; b_part <= n - 1; ++b_part) {

            idx_t g = ugcd(a_part, b_part);
            if (n % g != 0) continue;

            idx_t a1 = a_part / g;
            idx_t b1 = b_part / g;
            idx_t n1 = n / g;

            /* i0 is the smallest positive solution of a1*i ≡ n1 (mod b1) */
            idx_t inva = modinv(a1 % b1, b1);
            idx_t i0 = (n1 % b1) * inva % b1;
            if (i0 == 0) i0 = b1;          /* force i0 ≥ 1 */

            /* upper bound for i from j ≥ 1 */
            idx_t i_limit = (n - b_part) / a_part;
            if (i0 > i_limit) continue;

            idx_t C = (i_limit - i0) / b1 + 1;   /* number of solutions */
            seq_t C_seq = (seq_t)C;

            /* sum of i over all solutions */
            seq_t sum_i = C_seq * (seq_t)i0
                        + (seq_t)b1 * C_seq * (C_seq - 1) / 2;

            /* base_j = (n1 - a1*i0)/b1  (integer by construction) */
            idx_t base_j = (n1 - a1 * i0) / b1;
            seq_t sum_j = C_seq * (seq_t)base_j
                        - (seq_t)a1 * C_seq * (C_seq - 1) / 2;

            seq_t sum_k = sum_i + sum_j;   /* total parts for this pair */
            total += sum_k;
        }
    }
    return total;
}
