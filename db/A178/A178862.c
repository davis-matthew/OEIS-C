/*
 * A178862  Divisors of 20160.
 * https://oeis.org/A178862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, easy, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A178862: divisors of 20160 in increasing order. */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort used on seq_t values. Works for both C and C++. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* Static array holding all 84 divisors of 20160. */
static seq_t divisors[84];
static bool divisors_initialized = false;

/* Compute all divisors of 20160 and sort them. */
static void init_divisors(void)
{
    if (divisors_initialized) return;

    size_t idx = 0;
    for (int e2 = 0; e2 <= 6; ++e2) {
        seq_t v2 = 1;
        for (int i = 0; i < e2; ++i) v2 *= 2;
        for (int e3 = 0; e3 <= 2; ++e3) {
            seq_t v3 = v2;
            for (int i = 0; i < e3; ++i) v3 *= 3;
            for (int e5 = 0; e5 <= 1; ++e5) {
                seq_t v5 = v3;
                if (e5) v5 *= 5;
                for (int e7 = 0; e7 <= 1; ++e7) {
                    seq_t v7 = v5;
                    if (e7) v7 *= 7;
                    divisors[idx++] = v7;
                }
            }
        }
    }

    /* idx should now be 84 */
    qsort(divisors, 84, sizeof(seq_t), cmp_seq);
    divisors_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1; undefined for n=0 */

    init_divisors();

    if (n > 84) return (seq_t)0;        /* sequence terminates after 84 terms */

    return divisors[(size_t)(n - 1)];
}
