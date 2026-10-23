/*
 * A137849  Number of integers m from 1 through n inclusive such that
 *          d_i(n)<=d_i(m) for 1<=i<=Min(d(n),d(m)) where d_i(n) denotes the
 *          i-th smallest divisor of n and d(n) denotes the number of divisors
 *          of n (A000005).
 * https://oeis.org/A137849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by brute‑force enumeration of * all m ∈ {1,…,n}
 *            and checking that the sorted list of divisors * of n is
 *            component‑wise ≤ the sorted list of divisors of m * (for indices
 *            1 … min(d(n),d(m))). * (d(n) – number of divisors of n)
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

static void get_divisors(idx_t n, idx_t *div, size_t *cnt)
{
    /* Collect divisors in ascending order.
     * 'small' holds divisors up to sqrt(n),
     * 'large' holds the corresponding pair divisors in descending order. */
    idx_t small[256];
    idx_t large[256];
    size_t sc = 0, lc = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            small[sc++] = i;
            if (i * i != n) {
                large[lc++] = n / i;
            }
        }
    }

    size_t pos = 0;
    for (size_t i = 0; i < sc; ++i) {
        div[pos++] = small[i];
    }
    /* large[] currently contains larger divisors in descending order
     * reverse it to obtain ascending order. */
    for (size_t i = 0; i < lc; ++i) {
        div[pos++] = large[lc - 1 - i];
    }
    *cnt = pos;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined outside the sequence, but safe */

    /* Divisors of n – will be reused for each m */
    idx_t div_n[256];
    size_t cnt_n;
    get_divisors(n, div_n, &cnt_n);

    seq_t result = 0;
    idx_t div_m[256];

    for (idx_t m = 1; m <= n; ++m) {
        size_t cnt_m;
        get_divisors(m, div_m, &cnt_m);

        size_t minlen = (cnt_n < cnt_m) ? cnt_n : cnt_m;
        bool ok = true;
        for (size_t i = 0; i < minlen; ++i) {
            if (div_n[i] > div_m[i]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            result = result + ((seq_t)1);
        }
    }
    return result;
}
