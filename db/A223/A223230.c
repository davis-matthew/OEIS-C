/*
 * A223230  Rolling icosahedron footprints: number of n X 5 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves along an icosahedral edge.
 * https://oeis.org/A223230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A223230: Rolling icosahedron footprints
   Empirical recurrence (11‑term):
   a(n) = 251*a(n-1) -15873*a(n-2) +195175*a(n-3)+2042368*a(n-4)
          -47798442*a(n-5)+263976930*a(n-6) -451699632*a(n-7)
          -350892000*a(n-8)+1964608344*a(n-9) -2144659680*a(n-10)
          +765275040*a(n-11)
   Initial values (for n=1..11): 
   625, 75665, 11043445, 1697263985, 266409703885,
   42210625593305, 6716507564288245, 1070788920090847265,
   170861214141424879645, 27274345271819338522025,
   4354541372254902852256645. */

#include <stddef.h>
#include <stdlib.h>

/* local routine to convert a decimal string to seq_t */
static seq_t parse_uint(const char *s)
{
    seq_t v = 0;
    while (*s) {
        v = v * (seq_t)10 + (seq_t)(*s - '0');
        ++s;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined for n<1 */

    static seq_t *cache = NULL;      /* cache of terms  a(1)..a(len) */
    static size_t  cap   = 0;
    static size_t  len   = 0;        /* number of terms currently stored */

    size_t needed = (size_t)n;       /* terms needed: a(1)..a(n) */

    /* grow cache if necessary */
    if (cap < needed) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise first 11 terms if not already done */
    if (len < 11) {
        static const char *init_strs[11] = {
            "625", "75665", "11043445", "1697263985", "266409703885",
            "42210625593305", "6716507564288245", "1070788920090847265",
            "170861214141424879645", "27274345271819338522025",
            "4354541372254902852256645"
        };
        for (size_t i = len; i < 11 && i < needed; ++i)
            cache[i] = parse_uint(init_strs[i]);
        len = (len < 11) ? 11 : len;
        if (len > needed) len = needed;
    }

    /* compute terms beyond those already cached */
    if (len < needed) {
        /* coefficients are positive; the signs are applied below */
        const uint64_t pos_coeff[6]  = {251ULL, 195175ULL, 2042368ULL,
                                        263976930ULL, 1964608344ULL, 765275040ULL};
        const uint64_t neg_coeff[5]  = {15873ULL, 47798442ULL, 451699632ULL,
                                        350892000ULL, 2144659680ULL};
        const size_t pos_off[6] = {1,3,4,6,9,11};
        const size_t neg_off[5] = {2,5,7,8,10};

        /* two code paths: 64‑bit seq_t can use 128‑bit intermediates,
           otherwise rely on seq_t’s arbitrary‑precision operators. */
        if (sizeof(seq_t) <= 8) {
            for (size_t k = len; k < needed; ++k) {
                if (k < 11) continue;      /* already initialised */

                unsigned __int128 pos = 0, neg = 0;
                pos += (unsigned __int128)cache[k -   pos_off[0]] * pos_coeff[0];
                pos += (unsigned __int128)cache[k -   pos_off[1]] * pos_coeff[1];
                pos += (unsigned __int128)cache[k -   pos_off[2]] * pos_coeff[2];
                pos += (unsigned __int128)cache[k -   pos_off[3]] * pos_coeff[3];
                pos += (unsigned __int128)cache[k -   pos_off[4]] * pos_coeff[4];
                pos += (unsigned __int128)cache[k -   pos_off[5]] * pos_coeff[5];

                neg += (unsigned __int128)cache[k -   neg_off[0]] * neg_coeff[0];
                neg += (unsigned __int128)cache[k -   neg_off[1]] * neg_coeff[1];
                neg += (unsigned __int128)cache[k -   neg_off[2]] * neg_coeff[2];
                neg += (unsigned __int128)cache[k -   neg_off[3]] * neg_coeff[3];
                neg += (unsigned __int128)cache[k -   neg_off[4]] * neg_coeff[4];

                cache[k] = (seq_t)((__int128)(pos - neg));
            }
        } else {
            for (size_t k = len; k < needed; ++k) {
                if (k < 11) continue;      /* already initialised */

                seq_t pos = cache[k - pos_off[0]];
                pos *= pos_coeff[0];
                seq_t tmp = cache[k - pos_off[1]];
                tmp *= pos_coeff[1]; pos += tmp;
                tmp = cache[k - pos_off[2]];
                tmp *= pos_coeff[2]; pos += tmp;
                tmp = cache[k - pos_off[3]];
                tmp *= pos_coeff[3]; pos += tmp;
                tmp = cache[k - pos_off[4]];
                tmp *= pos_coeff[4]; pos += tmp;
                tmp = cache[k - pos_off[5]];
                tmp *= pos_coeff[5]; pos += tmp;

                seq_t neg = cache[k - neg_off[0]];
                neg *= neg_coeff[0];
                tmp = cache[k - neg_off[1]];
                tmp *= neg_coeff[1]; neg += tmp;
                tmp = cache[k - neg_off[2]];
                tmp *= neg_coeff[2]; neg += tmp;
                tmp = cache[k - neg_off[3]];
                tmp *= neg_coeff[3]; neg += tmp;
                tmp = cache[k - neg_off[4]];
                tmp *= neg_coeff[4]; neg += tmp;

                cache[k] = pos - neg;
            }
        }
        len = needed;
    }

    return cache[n-1];
}
