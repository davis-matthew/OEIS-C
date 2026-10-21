/*
 * A053568  Totients of nontotient numbers.
 * https://oeis.org/A053568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A007617(n))
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_N 300000  /* Upper bound large enough for n up to 10000 */

static seq_t phi[MAX_N + 1];
static bool   is_totient[MAX_N + 1];
static bool   initialized = false;

seq_t a(idx_t n)
{
    /* Initialise totient table and mark totient image values */
    if (!initialized) {
        initialized = true;

        /* Initialise phi[i] = i */
        for (idx_t i = 1; i <= MAX_N; ++i) {
            phi[i] = (seq_t)i;
            is_totient[i] = false;
        }

        /* Euler's totient sieve */
        for (idx_t i = 2; i <= MAX_N; ++i) {
            if (phi[i] == (seq_t)i) {            /* i is prime */
                for (idx_t j = i; j <= MAX_N; j += i) {
                    phi[j] -= phi[j] / (seq_t)i;
                }
            }
        }

        /* Mark numbers that appear as totients */
        for (idx_t j = 1; j <= MAX_N; ++j) {
            seq_t val = phi[j];
            if (val <= (seq_t)MAX_N) {
                is_totient[(idx_t)val] = true;
            }
        }
    }

    /* Find the nth nontotient and return its totient value */
    idx_t count = 0;
    for (idx_t v = 1; v <= MAX_N; ++v) {
        if (!is_totient[v]) {
            ++count;
            if (count == n) {
                return phi[v];
            }
        }
    }

    /* Should never reach here for n within the expected range */
    return (seq_t)0;
}
