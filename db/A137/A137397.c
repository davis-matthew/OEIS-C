/*
 * A137397  Number of distinct palindromic subwords in the binary
 *          representation of n.
 * https://oeis.org/A137397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count distinct palindromic subwords of the binary representation of n,
   including the empty string.  Return value is a(n). */
#include <stdbool.h>
#include <stddef.h>

/* Small structure to identify a distinct palindrome by its value and length. */
struct Pal {
    uint64_t val;
    unsigned char len;
};

/* Check whether a palindrome already occurs in the list. */
static bool pal_exists(struct Pal *list, size_t cnt,
                       uint64_t val, unsigned char len)
{
    for (size_t i = 0; i < cnt; ++i) {
        if (list[i].val == val && list[i].len == len)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Determine binary length L (treat n=0 as a single bit '0'). */
    unsigned L;
    if (n == 0) {
        L = 1;
    } else {
        uint64_t x = (uint64_t)n;
        L = 0;
        while (x) {
            ++L;
            x >>= 1;
        }
    }

    /* Extract bits of n into an array (most‑significant bit first). */
    uint8_t bits[64];
    uint64_t tmp = (uint64_t)n;
    for (unsigned i = 0; i < L; ++i) {
        bits[i] = (uint8_t)((tmp >> (L - 1 - i)) & 1ULL);
    }

    /* Store distinct palindromic substrings.  Max possible distinct substrings
       for L=64 is 2080; enough for all n that fit in 64‑bit. */
    struct Pal palbuf[2080];
    size_t palcnt = 0;

    for (unsigned i = 0; i < L; ++i) {
        for (unsigned j = i; j < L; ++j) {
            /* Check if bits[i..j] are a palindrome. */
            unsigned l = i, r = j;
            bool ispal = true;
            while (l < r) {
                if (bits[l] != bits[r]) {
                    ispal = false;
                    break;
                }
                ++l;
                --r;
            }
            if (!ispal)
                continue;

            /* Compute the integer value of the substring. */
            uint64_t val = 0;
            for (unsigned k = i; k <= j; ++k) {
                val = (val << 1) | bits[k];
            }

            /* Record distinct substring. */
            if (!pal_exists(palbuf, palcnt, val,
                            (unsigned char)(j - i + 1))) {
                palbuf[palcnt].val = val;
                palbuf[palcnt].len = (unsigned char)(j - i + 1);
                ++palcnt;
            }
        }
    }

    /* Add 1 for the empty subword. */
    return (seq_t)(palcnt + 1);
}
