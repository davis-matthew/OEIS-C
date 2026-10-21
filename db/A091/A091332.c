/*
 * A091332  Natural numbers written out with their digits grouped in sets of
 *          four (leading zeros omitted).
 * https://oeis.org/A091332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the 4‑digit block of the concatenation of the positive integers
 *            (i.e., digits positions 4n−3 .. 4n in the stream 123456789101112…).
 */
#include <stdint.h>

static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

static inline uint8_t get_digit(uint64_t pos) /* 1‑based position in the stream */
{
    uint64_t pos0 = pos - 1;                  /* zero‑based */
    for (int L = 1; ; ++L) {
        unsigned __int128 block = (unsigned __int128)9 * pow10[L-1] * L;
        if (pos0 < block) {
            uint64_t numberOffset = pos0 / L;
            int digitIndex = pos0 % L;          /* 0 … L-1 */
            uint64_t number = pow10[L-1] + numberOffset;
            uint64_t pow10_n = pow10[L - digitIndex - 1];
            return (uint8_t)((number / pow10_n) % 10);
        }
        pos0 -= (uint64_t)block;
    }
}

seq_t a(idx_t n)
{
    unsigned __int128 tmp = (unsigned __int128)4 * n - 3;
    uint64_t startpos = (uint64_t)tmp;      /* assumes n is small enough for 64‑bit */
    seq_t val = 0;
    for (int i = 0; i < 4; ++i) {
        uint8_t d = get_digit(startpos + i);
        val = val * 10 + d;
    }
    return val;
}
