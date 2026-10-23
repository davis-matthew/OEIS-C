/*
 * A161903  Convert n into a sequence of binary digits, apply one step of the
 *          rule 110 cellular automaton, and interpret the results as a binary
 *          integer.
 * https://oeis.org/A161903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = result of one step of rule 110 applied to the binary
 *            expansion of n, padded with a leading 0, interpreted as a binary
 *            integer
 * Range:     u8 n <= 127, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Rule‑110 truth table, indexed by the 3‑bit pattern L C R */
static const uint8_t RULE110[8] = {0, 1, 1, 1, 0, 1, 1, 0};

seq_t a(idx_t n)
{
    /* Find the number of bits in n (0 for n==0). */
    int L = 0;
    if (n != 0) {
        for (int i = 63; i >= 0; --i) {
            if ((n >> i) & 1ULL) {
                L = i + 1;
                break;
            }
        }
    }

    seq_t result = 0;
    uint8_t left  = 0;

    /* Process the input sequence of length L+1:
       bits[0] = 0 (leading zero), bits[1..L] = binary digits of n
       in the usual most‑significant‑first order. */
    for (idx_t pos = 0; pos <= (idx_t)L; ++pos) {
        uint8_t cur   = 0;
        uint8_t right = 0;

        /* Current bit */
        if (pos > 0)          /* positions 1..L correspond to bits of n */
            cur = (uint8_t)((n >> (L - pos)) & 1ULL);

        /* Right neighbor */
        if (pos < (idx_t)L)   /* positions 0..L-1 have a right neighbor */
            right = (uint8_t)((n >> (L - pos - 1)) & 1ULL);

        /* Apply the rule */
        uint8_t pattern = (left << 2) | (cur << 1) | right;
        seq_t  bit      = RULE110[pattern];

        /* Accumulate the result */
        result = (result << 1) | bit;

        left = cur;
    }

    return result;
}
