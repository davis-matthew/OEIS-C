/*
 * A161466  Divisors of 10!.
 * https://oeis.org/A161466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy, look
 * Formula:   a(n) = n-th smallest divisor of 10! (enumerated and sorted)
 * Range:     u8 none, u16 n <= 130, u32 n <= 270, u64 n <= 270, u128 n <= 270,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* number of divisors of 10! = 2^8 * 3^4 * 5^2 * 7^1 */
#define DIVISOR_COUNT 270

static seq_t *divisors = NULL;            /* sorted list of divisors */
static bool initialized = false;          /* flag */

/* comparator for qsort on seq_t */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!initialized) {
        /* allocate array */
        divisors = (seq_t *)malloc(DIVISOR_COUNT * sizeof(seq_t));
        if (divisors == NULL) {              /* allocation failed */
            /* return 0 on failure; unlikely in tests */
            return 0;
        }

        /* precompute primes powers */
        seq_t pow2[9], pow3[5], pow5[3], pow7[2];
        pow2[0] = pow3[0] = pow5[0] = pow7[0] = (seq_t)1;
        for (int i = 1; i <= 8; ++i) pow2[i] = pow2[i-1] * (seq_t)2;
        for (int i = 1; i <= 4; ++i) pow3[i] = pow3[i-1] * (seq_t)3;
        for (int i = 1; i <= 2; ++i) pow5[i] = pow5[i-1] * (seq_t)5;
        for (int i = 1; i <= 1; ++i) pow7[i] = pow7[i-1] * (seq_t)7;

        /* enumerate all divisors */
        size_t idx = 0;
        for (int e2 = 0; e2 <= 8; ++e2) {
            seq_t p2 = pow2[e2];
            for (int e3 = 0; e3 <= 4; ++e3) {
                seq_t p23 = p2 * pow3[e3];
                for (int e5 = 0; e5 <= 2; ++e5) {
                    seq_t p235 = p23 * pow5[e5];
                    for (int e7 = 0; e7 <= 1; ++e7) {
                        divisors[idx++] = p235 * pow7[e7];
                    }
                }
            }
        }
        /* idx should be DIVISOR_COUNT */
        (void)idx; /* silence unused warning if compiled with warnings */

        /* sort ascending */
        qsort(divisors, DIVISOR_COUNT, sizeof(seq_t), cmp_seq);

        initialized = true;
    }

    /* bounds check */
    if (n < 1 || n > DIVISOR_COUNT) {
        return 0; /* undefined for n > 270 */
    }
    return divisors[n - 1];
}
