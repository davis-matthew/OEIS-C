/*
 * A175479  a(1)=1. For n >= 1, if a(n) = 0, then append to the sequence the
 *          digits of binary n+1 (most significant digits first and least
 *          significant digits last). If a(n) = 1, then append to the sequence
 *          the digits of binary n+1 in reverse order (least significant
 *          digits first and most significant digits l...
 * https://oeis.org/A175479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175479: Sequentially build a binary sequence.
   a(1) = 1.
   For n ≥ 1: let a(n) be the bit at position n.
   If a(n) = 0, append the binary digits of n + 1 (MSB first).
   If a(n) = 1, append those digits in reverse order (LSB first). */

#include <stdlib.h>
#include <stdbool.h>

/* static linked list of bits, with current length and capacity */
static seq_t *bits = NULL;   /* array of bits (0 or 1) */
static size_t len  = 0;      /* number of bits generated so far */
static size_t cap  = 0;      /* allocated capacity in bits */
static idx_t processed = 0;  /* next index to process (1‑based) */

static void ensure_cap(size_t need)
{
    if (need > cap) {
        size_t newcap = (cap == 0) ? 64 : cap * 2;
        while (newcap < need) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(bits, newcap * sizeof(seq_t));
        bits = tmp;
        cap  = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 */

    /* Initialize on first call */
    if (!bits) {
        cap  = 64;
        bits = (seq_t *)malloc(cap * sizeof(seq_t));
        bits[0] = (seq_t)1;   /* a(1) = 1 */
        len = 1;
        processed = 1;        /* index 1 has been processed (produced bits for 2) */
    }

    /* Process indices 1 through n‑1 */
    while (processed < n) {
        seq_t curr = bits[processed - 1];   /* a(processed) */
        idx_t val  = processed + 1;         /* number whose binary digits are appended */

        if (curr == (seq_t)0) {   /* MSB first */
            /* Find number of bits of val */
            idx_t tmp = val;
            int bitlen = 0;
            while (tmp > 0) {
                bitlen++;
                tmp >>= 1;
            }
            for (int i = bitlen - 1; i >= 0; --i) {
                seq_t bit = (seq_t)((val >> i) & 1);
                ensure_cap(len + 1);
                bits[len++] = bit;
            }
        } else {                  /* LSB first (reversed) */
            idx_t tmp = val;
            while (tmp > 0) {
                seq_t bit = (seq_t)(tmp & 1);
                ensure_cap(len + 1);
                bits[len++] = bit;
                tmp >>= 1;
            }
        }
        processed++;
    }

    return bits[n - 1];
}
