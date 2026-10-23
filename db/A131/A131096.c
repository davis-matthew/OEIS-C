/*
 * A131096  3-smooth numbers in ternary representation.
 * https://oeis.org/A131096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal representation of the base‑3 digits of the n‑th
 *            3‑smooth number (numbers of the form 2^i·3^j)
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 92, u64 n <= 331,
 *            u128 n <= 1256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *smooth = NULL;          /* cache of 3‑smooth numbers   */
static size_t smooth_len = 0;         /* number of cached terms      */
static size_t smooth_cap = 0;         /* capacity of smooth array    */
static size_t idx2 = 0, idx3 = 0;     /* indices for the 2‑ and 3‑multiples */
static seq_t next2 = 0, next3 = 0;    /* next candidates              */
static bool initialized = false;      /* has the cache been initialized? */

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; n == 0 is outside its domain. */
    if (n == 0)
        return (seq_t)0;

    /* Lazy initialization of the cache. */
    if (!initialized) {
        smooth_cap = 1;
        smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        if (!smooth)
            return (seq_t)0;  /* allocation failure – return 0 */
        smooth[0] = (seq_t)1;            /* 2^0·3^0                     */
        smooth_len = 1;
        idx2 = 0;
        idx3 = 0;
        next2 = smooth[0] * (seq_t)2;    /* first multiple of 2 */
        next3 = smooth[0] * (seq_t)3;    /* first multiple of 3 */
        initialized = true;
    }

    /* Grow the cache up to the requested index. */
    while (smooth_len < (size_t)n) {
        /* Ensure there is room for the new element. */
        if (smooth_len + 1 > smooth_cap) {
            size_t new_cap = smooth_cap ? smooth_cap * 2 : 1;
            if (new_cap < smooth_len + 1)
                new_cap = smooth_len + 1;
            seq_t *tmp = (seq_t *)realloc(smooth, new_cap * sizeof(seq_t));
            if (!tmp)  /* allocation failure – abort with 0 */
                return (seq_t)0;
            smooth = tmp;
            smooth_cap = new_cap;
        }

        /* Choose the next smallest 3‑smooth number. */
        seq_t candidate = next2 < next3 ? next2 : next3;
        smooth[smooth_len] = candidate;
        ++smooth_len;

        /* Update the 2‑ and/or 3‑multiple indices if they were used. */
        if (candidate == next2) {
            ++idx2;
            next2 = smooth[idx2] * (seq_t)2;
        }
        if (candidate == next3) {
            ++idx3;
            next3 = smooth[idx3] * (seq_t)3;
        }
    }

    /* Convert the nth 3‑smooth number into the required decimal integer. */
    seq_t val = smooth[(size_t)n - 1];
    seq_t result = 0;
    seq_t mult    = 1;

    while (val > (seq_t)0) {
        unsigned digit = (unsigned)(val % (seq_t)3);   /* digit 0,1,2 */
        result += digit * mult;
        mult   *= (seq_t)10;
        val    /= (seq_t)3;
    }

    return result;
}
