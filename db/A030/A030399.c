/*
 * A030399  Write n in base 4, then complement each digit (d -> 3-d) and
 *          juxtapose.
 * https://oeis.org/A030399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A030399:  a(n) is the n‑th digit in the infinite stream
   obtained by writing each natural number in base 4, complementing
   each digit (d → 3−d), and concatenating the results. */

#include <stdint.h>

static const unsigned long long POW4[32] = {
    1ULL,                                    /* 4⁰  */
    4ULL,                                    /* 4¹  */
    16ULL,                                   /* 4²  */
    64ULL,                                   /* 4³  */
    256ULL,                                  /* 4⁴  */
    1024ULL,                                 /* 4⁵  */
    4096ULL,                                 /* 4⁶  */
    16384ULL,                                /* 4⁷  */
    65536ULL,                                /* 4⁸  */
    262144ULL,                               /* 4⁹  */
    1048576ULL,                              /* 4¹⁰ */
    4194304ULL,                              /* 4¹¹ */
    16777216ULL,                             /* 4¹² */
    67108864ULL,                             /* 4¹³ */
    268435456ULL,                            /* 4¹⁴ */
    1073741824ULL,                           /* 4¹⁵ */
    4294967296ULL,                           /* 4¹⁶ */
    17179869184ULL,                          /* 4¹⁷ */
    68719476736ULL,                          /* 4¹⁸ */
    274877906944ULL,                         /* 4¹⁹ */
    1099511627776ULL,                        /* 4²⁰ */
    4398046511104ULL,                        /* 4²¹ */
    17592186044448ULL,                       /* 4²² */
    70368744177664ULL,                       /* 4²³ */
    281474976710656ULL,                      /* 4²⁴ */
    1125899906842624ULL,                     /* 4²⁵ */
    4503599627370496ULL,                     /* 4²⁶ */
    18014398509481984ULL,                    /* 4²⁷ */
    72057594037927936ULL,                    /* 4²⁸ */
    288230376151711744ULL,                   /* 4²⁹ */
    1152921504606846976ULL                   /* 4³⁰ */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    idx_t rem = n;                 /* 1‑based position in the stream */

    int l = 1;                     /* current length in base‑4 digits */
    for (; l < 32; ++l) {
        idx_t count = (l == 1) ? (idx_t)3
                               : (idx_t)(3ULL * POW4[l - 1]);

        unsigned __int128 block =
            (unsigned __int128)count * (unsigned __int128)l;

        if ((unsigned __int128)rem <= block)
            break;
        rem -= (idx_t)block;
    }

    /* 'l' is the number of digits of the integer containing the desired digit.
       'rem' (1‑based) is its position within the current block. */
    idx_t offset     = rem - 1;                       /* zero‑based offset */
    idx_t num_index  = offset / (idx_t)l;             /* which number */
    unsigned digit_pos =
        (unsigned)(offset % (idx_t)l);                /* position from MSB */

    idx_t number = POW4[l - 1] + num_index;           /* actual integer   */
    idx_t power  = POW4[l - 1 - digit_pos];           /* 4^(l-1-digit_pos)*/
    idx_t digit  = (number / power) % (idx_t)4;       /* base‑4 digit     */

    return (seq_t)(3 - digit);                       /* complemented digit */
}
