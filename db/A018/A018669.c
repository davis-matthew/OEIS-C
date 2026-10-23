/*
 * A018669  Divisors of 828.
 * https://oeis.org/A018669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 828 sorted ascending */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t divs[18];
static bool divs_ready = false;

static void init_divs(void)
{
    static const uint64_t primes[3] = {2, 3, 23};
    static const uint8_t max_exp[3] = {2, 2, 1};

    seq_t temp[18];
    size_t count = 0;

    for (uint8_t e0 = 0; e0 <= max_exp[0]; ++e0) {
        for (uint8_t e1 = 0; e1 <= max_exp[1]; ++e1) {
            for (uint8_t e2 = 0; e2 <= max_exp[2]; ++e2) {
                seq_t val = 1;
                for (uint8_t k = 0; k < e0; ++k) val *= primes[0];
                for (uint8_t k = 0; k < e1; ++k) val *= primes[1];
                for (uint8_t k = 0; k < e2; ++k) val *= primes[2];
                temp[count++] = val;
            }
        }
    }

    /* Simple insertion sort (max 17 passes) */
    for (int i = 1; i < (int)count; ++i) {
        seq_t key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > key) {
            temp[j + 1] = temp[j];
            --j;
        }
        temp[j + 1] = key;
    }

    for (size_t i = 0; i < count; ++i) {
        divs[i] = temp[i];
    }

    divs_ready = true;
}

seq_t a(idx_t n)
{
    if (!divs_ready) {
        init_divs();
    }
    if (n < 1 || n > 18) {
        return (seq_t)0;
    }
    return divs[(size_t)(n - 1)];
}
