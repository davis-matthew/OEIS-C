/*
 * A010081  Weight distribution of extended Hamming code of length 32 (or
 *          3rd-order Reed-Muller code).
 * https://oeis.org/A010081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of all 2^26 codewords of the [32,26] extended Hamming code. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Parity‑contribution masks for the 26 information bits.
   Each entry is a 5‑bit mask indicating which of the 5 parity bits
   a given data bit contributes to.  The masks are all non‑zero
   5‑bit vectors, i.e. the set of columns of the H matrix. */
static const unsigned char Pmask[26] = {
    0x03, 0x05, 0x06, 0x07, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
    0x0f, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

seq_t a(idx_t n)
{
    static seq_t counts[17] = {0};
    static bool init = false;

    if (!init) {
        init = true;
        const idx_t limit = ((idx_t)1 << 26);     /* 2^26 messages */
        idx_t i;

        uint64_t m = 0;          /* current message bits (0..25) */
        unsigned char parity = 0; /* cumulative parity bits (5 bits) */
        int popcnt_m = 0;        /* popcount of m */
        uint64_t gray_prev = 0;  /* previous gray code */

        for (i = 0; i < limit; ++i) {
            if (i > 0) {
                uint64_t gray = i ^ (i >> 1);
                uint64_t diff = gray ^ gray_prev;
                unsigned bit = __builtin_ctzll(diff); /* changed bit index */

                /* Flip the data bit and update its population count. */
                m ^= ((uint64_t)1 << bit);
                if ((m >> bit) & 1ULL)
                    ++popcnt_m;
                else
                    --popcnt_m;

                /* Update parity bits (XOR with the column mask). */
                parity ^= Pmask[bit];

                gray_prev = gray;
            }

            int popcnt_parity = __builtin_popcount((unsigned)parity);
            int sum = popcnt_m + popcnt_parity;
            int weight = sum + (sum & 1);   /* overall parity makes weight even */
            unsigned idx = weight >> 1;     /* weight/2 -> table index */

            ++counts[idx];
        }
    }

    if (n > 16) return 0;
    return counts[(size_t)n];
}
