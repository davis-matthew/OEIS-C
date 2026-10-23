/*
 * A043735  Numbers whose number of runs in the base-2 representation is
 *          congruent to 2 mod 6.
 * https://oeis.org/A043735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th positive integer whose binary representation
 * contains a number of runs (maximal blocks of equal bits) congruent to 2
 * modulo 6.  Runs are counted without leading zeros. */

#include <stdbool.h>
#include <stddef.h>

/* Return the bit length of a non‑negative seq_t value (number of bits
 * needed to represent it, 0 for value 0). */
static inline size_t bitlen_seq_t(seq_t x)
{
    size_t len = 0;
    while (x != (seq_t)0) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Determine whether a seq_t number has run‑count ≡ 2 (mod 6). */
static inline bool runs_mod2(seq_t v)
{
    size_t len = bitlen_seq_t(v);
    if (len == 0)
        return false;                     /* 0 has 0 runs, 0 ≡ 0 mod 6 */

    bool started = false;
    int prev_bit = 0;
    size_t runs = 0;

    /* Inspect bits from most significant to least significant. */
    for (size_t i = len; i > 0; --i) {
        int bit = (int)((v >> (i - 1)) & (seq_t)1);
        if (!started) {                  /* first 1 bit starts the number */
            started = true;
            prev_bit = bit;
            runs = 1;
            continue;
        }
        if (bit != prev_bit) {
            runs++;
            prev_bit = bit;
        }
    }
    return ((runs % 6) == 2);
}

/* Return the n‑th term of the sequence, n ≥ 1.  The function works for
 * any build (uint8/16/32/64 bits or arbitrary‑precision classes). */
seq_t a(idx_t n)
{
    if (n == 0)                /* out of range */
        return (seq_t)0;

    seq_t x = (seq_t)1;
    idx_t found = 0;

    /* Simple linear search.  Even for n = 10 000 the search
     * stops below the millions, which is easily fast enough. */
    while (true) {
        x = x + (seq_t)1;              /* advance to the next integer */
        if (runs_mod2(x)) {
            found++;
            if (found == n)
                return x;
        }
    }
}
