/*
 * A057853  Number of letters in n (in Esperanto).
 * https://oeis.org/A057853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of letters in the Esperanto word for n.
   For n=0 it returns 3 ("nul").
   For n>0, n is decomposed into 3‑digit groups (units, thousands,
   millions, billions, trillions, quadrillions, kvintillions).
   The length of each group word is added plus the appropriate scale
   word ("mil", "miliono", "miliardo", "triliono", "kvadriliono",
   "kvintiliono") in their singular or plural form. */

#include <stdint.h>

static const unsigned char DIGIT_LEN[10]  = {3,3,2,3,4,4,3,3,2,3};
static const unsigned char TENS_LEN[10]   = {0,3,5,6,7,7,6,6,5,6};
static const unsigned char HUNDRED_LEN[10]= {0,4,6,7,8,8,7,7,6,7};

static const unsigned char SINGLE_SCALE_LEN[7] = {0,3,7,8,8,11,11};
static const unsigned char PLURAL_SCALE_LEN[7] = {0,3,8,9,9,12,12};

static inline seq_t block_len(uint64_t val) {
    seq_t len = 0;
    uint64_t h = val / 100;
    if (h > 0) {
        if (h == 1) len += HUNDRED_LEN[1];   /* "cent" */
        else        len += HUNDRED_LEN[(int)h];
    }

    uint64_t r = val % 100;
    if (r < 10) {
        len += DIGIT_LEN[r];
    } else if (r == 10) {
        len += TENS_LEN[1];                 /* "dek" */
    } else if (r < 20) {
        len += TENS_LEN[1];                 /* "dek" */
        len += DIGIT_LEN[r-10];
    } else {
        uint64_t t = r / 10;
        uint64_t u = r % 10;
        len += TENS_LEN[t];
        if (u > 0) len += DIGIT_LEN[u];
    }
    return len;
}

seq_t a(idx_t n) {
    if (n == 0) return 3; /* "nul" */

    seq_t total = 0;
    idx_t block = n;
    int k = 0; /* group index: 0 units, 1 thousands, 2 millions, ... */
    while (block > 0) {
        uint64_t val = block % 1000;
        block /= 1000;
        if (val == 0) {
            ++k;
            continue;
        }

        seq_t len = block_len(val);
        if (k == 0) {                 /* units */
            total += len;
        } else if (k == 1) {          /* thousands */
            if (val == 1) {
                total += 3;           /* "mil" */
            } else {
                total += len + 3;     /* value word + "mil" */
            }
        } else {                      /* millions and above */
            if (val == 1) {
                total += SINGLE_SCALE_LEN[k];
            } else {
                total += len + PLURAL_SCALE_LEN[k];
            }
        }
        ++k;
    }
    return total;
}
