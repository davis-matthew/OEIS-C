/*
 * A008540  Numbers whose American English name contains the letter 'f'.
 * https://oeis.org/A008540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose American English name contains the letter 'f' */
/*   The function searches for the nth number whose name includes 'f'. */

#include <stdbool.h>
#include <stdint.h>

/* Small numbers 0..19 */
static const bool small_f[20] = {
    false, /* zero */
    false, /* one */
    false, /* two */
    false, /* three */
    true,  /* four */
    true,  /* five */
    false, /* six */
    false, /* seven */
    false, /* eight */
    false, /* nine */
    false, /* ten */
    false, /* eleven */
    false, /* twelve */
    false, /* thirteen */
    true,  /* fourteen */
    true,  /* fifteen */
    false, /* sixteen */
    false, /* seventeen */
    false, /* eighteen */
    false  /* nineteen */
};

/* Tens 0..9 (20,30,...,90) */
static const bool tens_f[10] = {
    false, /* 0 */
    false, /* 1 (10-19) */
    false, /* 20 */
    false, /* 30 */
    true,  /* 40 (forty) */
    true,  /* 50 (fifty) */
    false, /* 60 */
    false, /* 70 */
    false, /* 80 */
    false  /* 90 */
};

/* Helper: returns true if the American English word for n contains 'f' */
static bool has_f_part(uint64_t n)
{
    if (n == 0) return false;

    if (n >= 1000) {
        uint64_t thousands = n / 1000;
        if (has_f_part(thousands)) return true;
        n %= 1000;
    }

    if (n >= 100) {
        if (small_f[n / 100]) return true;
        n %= 100;
    }

    if (n >= 20) {
        if (tens_f[n / 10]) return true;
        n %= 10;
        if (n && small_f[n]) return true;
    } else if (n > 0) {
        if (small_f[n]) return true;
    }

    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n=0 is arbitrary */

    idx_t count = 0;
    uint64_t num = 0;

    while (1) {
        num++;
        if (has_f_part(num)) {
            count++;
            if (count == n)
                return (seq_t)num;
        }
    }
}
