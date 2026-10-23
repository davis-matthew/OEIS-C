/*
 * A186041  Numbers of the form 3*k + 2, 5*k + 3, or 7*k + 4.
 * https://oeis.org/A186041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-57) + 105 for n > 57. a(1)..a(57) are the first 57
 *            integers ≥2 that satisfy x ≡ 2 (mod 3) or x ≡ 3 (mod 5) or x ≡ 4
 *            (mod 7)
 * Range:     u8 n <= 138, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint64_t and friends */

static bool seq_initialized = false;
static seq_t seq_values[57];

/* Compute the first 57 terms once. */
static void init_seq(void) {
    if (seq_initialized) return;
    idx_t num = 2;
    size_t count = 0;
    while (count < 57) {
        if ((num % 3 == 2) || (num % 5 == 3) || (num % 7 == 4)) {
            seq_values[count++] = (seq_t)num;
        }
        ++num;
    }
    seq_initialized = true;
}

/* Main function to return the nth term. */
seq_t a(idx_t n) {
    init_seq();
    if (n <= 57) {
        return seq_values[n - 1];
    }
    idx_t q   = (n - 1) / 57;                     /* number of full periods */
    size_t i  = (size_t)((n - 1) % 57);           /* position within period */
    return seq_values[i] + (seq_t)q * (seq_t)105;
}
