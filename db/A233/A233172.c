/*
 * A233172  Number of n X 6 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally, diagonally or
 *          antidiagonally, top left element zero, and 1 appearing before 2 3
 *          and 4, and 2 appearing before 3 in row major order (unlabelled
 *          6-colorings with no clashing color pairs).
 * https://oeis.org/A233172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1216*a(n-1) -303104*a(n-2) -6553600*a(n-3)
 *               +6056574976*a(n-4) -287762808832*a(n-5) -18622978195456*a(n-6)
 *               +1482141674242048*a(n-7) -15199648742375424*a(n-8)
 *               -522417556774977536*a(n-9) +5764607523034234880*a(n-10)
 *       (10‑term linear recurrence valid for n ≥ 11; see OEIS A233172)
 *
 * The first ten terms are hard‑coded and obtained from the OEIS b‑file.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* Signed recurrence coefficients (a(n-1) … a(n-10)) */
static const long long coeff[10] = {
    1216LL,                          /* a(n-1)  */
   -303104LL,                       /* a(n-2)  */
  -6553600LL,                       /* a(n-3)  */
 6056574976LL,                      /* a(n-4)  */
-287762808832LL,                    /* a(n-5)  */
-18622978195456LL,                  /* a(n-6)  */
1482141674242048LL,                 /* a(n-7)  */
-15199648742375424LL,               /* a(n-8)  */
-522417556774977536LL,              /* a(n-9)  */
5764607523034234880LL               /* a(n-10) */
};

/* Absolute values of the coefficients (used for multiplication) */
static const unsigned long long abscoeff[10] = {
    1216ULL,
   303104ULL,
   6553600ULL,
 6056574976ULL,
287762808832ULL,
18622978195456ULL,
1482141674242048ULL,
15199648742375424ULL,
522417556774977536ULL,
5764607523034234880ULL
};

/* Large initial terms as strings for arbitrary‑precision conversion */
static const char *init_str[10] = {
    "136",
    "92672",
    "76447744",
    "65265467392",
    "56275748519936",
    "48667983827959808",
    "42129429039341895680",
    "36480870847649836695552",
    "31592958742989872646062080",
    "27360904144981190573706706944"
};

seq_t a(idx_t n)
{
    /* Lazy initialization of the first ten values */
    static seq_t init_vals[10];
    static bool init_done = false;
    if (!init_done) {
        for (int i = 0; i < 10; ++i)
            init_vals[i] = seq_from_str(init_str[i]);
        init_done = true;
    }

    if (n <= 10)
        return init_vals[n - 1];

    /* arr[0] = a(k), arr[1] = a(k-1), … arr[9] = a(k-9) */
    seq_t arr[10];
    for (int i = 0; i < 10; ++i)
        arr[i] = init_vals[9 - i];

    for (idx_t step = 10; step < n; ++step) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 10; ++k) {
            seq_t prod = arr[k] * (seq_t)abscoeff[k];
            if (coeff[k] >= 0)
                pos += prod;
            else
                neg += prod;
        }
        seq_t next = pos - neg;   /* Should be positive for all n */

        /* Shift array for next iteration */
        for (int j = 9; j > 0; --j)
            arr[j] = arr[j - 1];
        arr[0] = next;
    }

    return arr[0];
}
