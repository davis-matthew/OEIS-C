/*
 * A104142  Take any three consecutive terms and write them down (in English):
 *          their letters sum up to a square number. This is the slowest
 *          strictly increasing such sequence.
 * https://oeis.org/A104142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   a(n) is the n-th smallest integer such that the total number of
 *            letters in the English names of any three consecutive terms is a
 *            perfect square; the names are written without “and”, hyphens, or
 *            spaces
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------------
  Helper tables for English words (no “and”, no hyphens).
 --------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Numeric word lengths (in letters).           zero … nine             */
static const uint8_t len_units[10] = {4,3,3,5,4,4,3,5,5,4};

/* ten…nineteen  */
static const uint8_t len_teens[10] = {3,6,6,8,8,7,7,9,8,8};

/* twenty…ninety  */
static const uint8_t len_tens[10]  = {0,0,6,6,5,5,5,7,6,6};

/* thousand, million, … (12 groups: 0 dummy) */
static const uint8_t len_scales[12] = {
    0,    /* dummy for group 0 */
    8,    /* thousand */
    7,    /* million */
    8,    /* billion */
    8,    /* trillion */
    11,   /* quadrillion */
    11,   /* quintillion */
    10,   /* sextillion */
    10,   /* septillion */
    9,    /* octillion */
    9,    /* nonillion */
    9     /* decillion */
};

/* All perfect squares up to 70² = 4900; this is well beyond the
   maximum sum of three word lengths for the range of interest.   */
static const uint32_t squares[71] = {
    0,1,4,9,16,25,36,49,64,81,100,121,144,169,196,225,256,289,324,361,
    400,441,484,529,576,625,676,729,784,841,900,961,1024,1089,1156,1225,
    1296,1369,1444,1521,1600,1681,1764,1849,1936,2025,2116,2209,2304,
    2401,2500,2601,2704,2809,2916,3025,3136,3249,3364,3481,3600,3721,
    3844,3969,4096,4225,4356,4489,4624,4761,4900
};

/* Test if a 32‑bit value is a perfect square.  The list above is
   sorted, so a simple linear search is fast enough. */
static inline bool is_square(uint32_t v)
{
    for (int i = 0; squares[i] <= v; ++i)
        if (squares[i] == v) return true;
    return false;
}

/*--------------------------------------------------------------------
  Letter count of an integer written in English.
  The function works for any `seq_t` type; all arithmetic is done
  using that type.  No “and”, no hyphens, no spaces.
  --------------------------------------------------------------------*/
static inline uint32_t len_word(seq_t n)
{
    if (n == 0) return 4;        /* “zero” */

    uint32_t total = 0;
    int grp = 0;                 /* 0 = ones, 1 = thousands, ... */
    seq_t tmp = n;
    const seq_t one      = (seq_t)1;
    const seq_t hundred  = (seq_t)100;
    const seq_t thousand = (seq_t)1000;
    const seq_t ten      = (seq_t)10;

    while (tmp > 0) {
        seq_t grp_val = tmp % thousand;
        if (grp_val) {
            uint32_t len_grp = 0;
            seq_t h = grp_val / hundred;
            seq_t rem = grp_val % hundred;

            if (h) {
                unsigned uh = (unsigned)h;  /* h < 10 for proper mapping */
                len_grp += len_units[uh];
                len_grp += 7;                /* “hundred” */
            }

            if (rem >= 10 && rem < 20) {
                unsigned th = (unsigned)(rem - 10);
                len_grp += len_teens[th];
            } else {
                seq_t t = rem / ten;
                seq_t u = rem % ten;
                if (t >= 2) {
                    unsigned tu = (unsigned)t;
                    len_grp += len_tens[tu];
                }
                if (u) {
                    unsigned uu = (unsigned)u;
                    len_grp += len_units[uu];
                }
            }

            if (grp && grp < 12) {
                len_grp += len_scales[grp];
            }
            total += len_grp;
        }

        tmp /= thousand;
        ++grp;
    }
    return total;
}

/*--------------------------------------------------------------------
  Dynamic storage for the sequence and the corresponding lengths.
  The vectors are grown only as needed, never shrunk.
  --------------------------------------------------------------------*/
static seq_t *seq_arr  = NULL;
static uint32_t *len_arr = NULL;
static size_t seq_size = 0;   /* number of elements stored */
static size_t seq_cap  = 0;   /* allocated capacity */

/* Ensure the arrays have capacity for at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_arr  = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    len_arr = (uint32_t *)realloc(len_arr, new_cap * sizeof(uint32_t));
    seq_cap = new_cap;
}

/*--------------------------------------------------------------------
  Main export: a(idx_t n)
  --------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */
    /* initialise storage once */
    if (seq_size == 0) {
        ensure_capacity(8);
        seq_arr[0] = (seq_t)1;   /* a(1) */
        seq_arr[1] = (seq_t)2;   /* a(2) */
        seq_arr[2] = (seq_t)6;   /* a(3) */
        seq_size = 3;

        len_arr[0] = len_word(seq_arr[0]);
        len_arr[1] = len_word(seq_arr[1]);
        len_arr[2] = len_word(seq_arr[2]);
    }

    /* Grow the sequence up to the required index. */
    while (seq_size < (size_t)n) {
        ensure_capacity(seq_size + 8);
        seq_t cand = seq_arr[seq_size - 1] + (seq_t)1;
        for (;;) {
            uint32_t lc = len_word(cand);
            uint32_t sum = len_arr[seq_size - 2] + len_arr[seq_size - 1] + lc;
            if (is_square(sum)) {
                seq_arr[seq_size] = cand;
                len_arr[seq_size] = lc;
                seq_size++;
                break;
            }
            cand = cand + (seq_t)1;
        }
    }
    return seq_arr[n - 1];
}
