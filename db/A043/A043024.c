/*
 * A043024  Base-8 palindromes that start with 4.
 * https://oeis.org/A043024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow_seq8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(idx_t n)
 * Computes the n-th base‑8 palindrome that starts with digit 4.
 * The sequence is produced in increasing decimal order.
 * The implementation runs in O(1) per call for n up to at least 10000. */
#include <stdint.h>      /* for uint64_t, if used in the build */
#include "../utils/pow_seq8.h"

seq_t a(idx_t n)
{
    /* Defensive guard: no guarantee when n == 0, but return 0. */
    if (n == (idx_t)0) return (seq_t)0;

    idx_t remaining = n;          /* how many palindromes still to skip */
    int L = 1;                    /* current length (in base‑8 digits) */

    while (1) {
        int half = (L + 1) / 2;   /* number of digits needed to specify a palindrome */
        int independent = half - 1; /* all except the mandated leading 4 */

        /* Count of palindromes with this length: 8^independent. */
        idx_t count = 1;
        for (int i = 0; i < independent; ++i) count *= (idx_t)8;

        if (remaining <= count) {
            /* The desired palindrome is among those of length L. */
            idx_t idx = remaining - 1;  /* 0‑based index within this block */
            unsigned char digits[128];
            digits[0] = 4;             /* leading digit */

            /* Extract the independent digits in big‑endian order. */
            for (int pos = 1; pos <= independent; ++pos) {
                int exp = independent - pos;    /* how many lower independent digits */
                idx_t power = 1;
                for (int j = 0; j < exp; ++j) power *= (idx_t)8;
                digits[pos] = (unsigned char)(idx / power);
                idx %= power;
            }

            /* Mirror to form the full palindrome. */
            for (int i = half; i < L; ++i)
                digits[i] = digits[L - 1 - i];

            /* Convert base‑8 digits to a decimal value of type seq_t. */
            seq_t val = (seq_t)0;
            for (int i = 0; i < L; ++i)
                val = val * (seq_t)8 + (seq_t)digits[i];

            return val;
        }

        /* Skip all palindromes of this length and proceed to longer ones. */
        remaining -= count;
        ++L;
    }
}
