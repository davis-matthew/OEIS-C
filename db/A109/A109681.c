/*
 * A109681  "Sloping ternary numbers": write numbers in ternary under each
 *          other (right-justified), read diagonals in upward direction,
 *          convert to decimal.
 * https://oeis.org/A109681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ternary_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = value of the upward diagonal in the right‑justified
 *  triangular array of ternary digits of the natural numbers.
 *
 *  For each n let r be a row index and let c=r−n be the column index
 *  of the digit that lies on the diagonal r−c=n.  All rows r for
 *  which 0≤c<digits_in_base3(r) contain a digit d = (r/3^c) mod 3.
 *  The diagonal is read starting at the lowest row (largest r) and
 *  moving upward.  The sequence term a(n) is the decimal value of
 *  the number whose most significant digit is the digit from the
 *  lowest row and whose least significant digit is the digit from
 *  the row r=n.
 *
 *  Algorithm:
 *    1. Find the maximal row rmax such that rmax−n<digits_in_base3(rmax).
 *    2. Starting from rmax and descending to n, extract the digit
 *       d = (r / 3^(r−n)) mod 3 and build the result by
 *       result = result*3 + d.
 *
 *  Complexity:  O(log_3 n) per call.
 */

#include <stdint.h>
#include "../utils/ternary_len.h"

static const seq_t POW3[64] = { 1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL, 729ULL,
                               2187ULL, 6561ULL, 19683ULL, 59049ULL, 177147ULL,
                               531441ULL, 1594323ULL, 4782969ULL, 14348907ULL,
                               43046721ULL, 129140163ULL, 387420489ULL,
                               1162261467ULL, 3486784401ULL, 10460353203ULL,
                               31381059609ULL, 94143178827ULL, 282429536481ULL,
                               847288609443ULL, 2541865828329ULL, 7625597484987ULL,
                               22876792454961ULL, 68630377364823ULL,
                               205891132094649ULL, 617673396283947ULL,
                               1853020188851841ULL, 5559060566555523ULL,
                               16677181699666569ULL, 50031545098999707ULL,
                               150094635295999821ULL, 450283905887999463ULL,
                               1350851717763998390ULL, 4052555153281995170ULL,
                               12157665429945985530ULL, 36472996289837956590ULL,
                               109418989229513869770ULL, 328256967688541609311ULL,
                               984770903065624827933ULL, 2954312709196874482800ULL,
                               8862938127590623448400ULL, 26588814382771870345200ULL,
                               79766443148315611035600ULL, 239299329445946833106000ULL,
                               717897987691840499322000ULL, 2153693963075521497962000ULL,
                               6461081889226564493896000ULL, 19383245667679793481688000ULL
};

/* Main sequence function */
seq_t a(idx_t n) {
    /* 1. Find the highest row rmax on diagonal n. */
    idx_t r = n;
    idx_t rmax = n;
    while (1) {
        idx_t c = r - n;                 /* column index on this diagonal */
        idx_t lenr = ternary_len(r);
        if (c < lenr) {
            rmax = r;
            r++;
        } else {
            break;
        }
    }

    /* 2. Collect digits from rmax down to n. */
    seq_t result = 0;
    for (idx_t rr = rmax; ; rr--) {
        idx_t c = rr - n;
        seq_t digit = ((seq_t)rr / POW3[c]) % 3;
        result = result * 3 + digit;
        if (rr == n) break;
    }
    return result;
}
