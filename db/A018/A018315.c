/*
 * A018315  Divisors of 170.
 * https://oeis.org/A018315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 170: a(n) is the n‑th divisor of 170 in increasing order */
#include <stdint.h>
#include <stdbool.h>

static const seq_t primes[3] = {2,5,17};
static const unsigned int exps[3] = {1,1,1};

static seq_t divisor_table[8];
static bool div_initialized = false;

static void compute_divisors(void)
{
    seq_t tmp[8];
    unsigned int idx = 0;
    for (unsigned int e0 = 0; e0 <= exps[0]; ++e0) {
        seq_t val0 = 1;
        for (unsigned int k = 0; k < e0; ++k) val0 *= primes[0];
        for (unsigned int e1 = 0; e1 <= exps[1]; ++e1) {
            seq_t val1 = val0;
            for (unsigned int k = 0; k < e1; ++k) val1 *= primes[1];
            for (unsigned int e2 = 0; e2 <= exps[2]; ++e2) {
                seq_t val = val1;
                for (unsigned int k = 0; k < e2; ++k) val *= primes[2];
                tmp[idx++] = val;
            }
        }
    }
    /* simple bubble sort for small fixed array */
    for (unsigned int i = 0; i < 8; ++i) {
        for (unsigned int j = i + 1; j < 8; ++j) {
            if (tmp[i] > tmp[j]) {
                seq_t c = tmp[i];
                tmp[i] = tmp[j];
                tmp[j] = c;
            }
        }
    }
    for (unsigned int i = 0; i < 8; ++i) divisor_table[i] = tmp[i];
    div_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1 */
    if (!div_initialized) compute_divisors();
    if (n >= 1 && n <= 8) return divisor_table[n - 1];
    return 0;
}
