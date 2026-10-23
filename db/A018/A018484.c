/*
 * A018484  Divisors of 492.
 * https://oeis.org/A018484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 492, in ascending order
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for the pre‑computed divisors of 492. */
static bool initialized = false;
static idx_t divisors[64];
static size_t num_divs = 0;

/* Comparator for idx_t values (used by qsort). */
static int cmp_idx(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* Compute the sorted list of all divisors of 492. */
static void init_divisors(void)
{
    if (initialized)
        return;

    idx_t N = (idx_t)492;
    /* Simple integer square root computation (since N is tiny). */
    idx_t sqrt_n = 0;
    for (idx_t i = 1; ; ++i) {
        if ((i + 1) * (i + 1) > N) {
            sqrt_n = i;
            break;
        }
    }

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (N % i == 0) {
            divisors[num_divs++] = i;
            idx_t other = N / i;
            if (other != i)
                divisors[num_divs++] = other;
        }
    }

    /* Sort into ascending order. */
    qsort(divisors, num_divs, sizeof(idx_t), cmp_idx);
    initialized = true;
}

/* Return the n-th divisor of 492. */
seq_t a(idx_t n)
{
    init_divisors();
    if (n == 0 || n > num_divs)
        return (seq_t)0;             /* undefined for out‑of‑range requests */
    return (seq_t)divisors[n - 1];
}
