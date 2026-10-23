/*
 * A018770  Divisors of 1004.
 * https://oeis.org/A018770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A018770: divisors of 1004 in ascending order. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* pre‑computed list of divisors of 1004, built only once */
    static seq_t divs[16];
    static size_t num = 0;
    static int built = 0;

    if (!built) {
        const idx_t N = 1004ULL;          /* the fixed integer to factor */
        for (size_t i = 1; i <= (size_t)N; ++i) {
            if (N % i == 0)
                divs[num++] = (seq_t)i;
        }
        built = 1;
    }

    if (n >= 1 && (size_t)(n - 1) < num)
        return divs[(size_t)(n - 1)];
    return (seq_t)0;   /* value for indices beyond the sequence */
}
