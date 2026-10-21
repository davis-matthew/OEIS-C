/*
 * A074156  Smallest multiple of n with initial digits that of the reversal of
 *          n, deleting the leading zeros wherever required.
 * https://oeis.org/A074156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = smallest multiple of n whose decimal representation
 *            begins with the digit reversal of n (leading zeros of the
 *            reversal discarded). Implementation uses modular arithmetic to
 *            find the minimal number of appended zeros
 * Range:     u8 n <= 12, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires: seq_t and idx_t defined by the build (int, long long,
   unsigned long long, unsigned __int128, etc.).  No external functions
   are called.  The function works for any n >= 1. */
seq_t a(idx_t n)
{
    /* Handle the trivial case; n==0 never occurs in the ACF but guard anyway */
    if (n == 0) return 0;

    /* Step 1: compute the reversal of n without leading zeros */
    seq_t rev = 0;
    seq_t tmp = (seq_t)n;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* Step 2: iterate over the possible number of appended zeros (d) */
    seq_t n_mod = (seq_t)n;                /* to use with seq_t arithmetic */
    seq_t rev_mod = rev % n_mod;           /* rev * 10^0 mod n */
    seq_t pow10 = 1;                       /* 10^d as seq_t */
    seq_t rev_pref_mod = rev_mod;          /* rev * 10^d mod n at each d */

    for (;;) { /* d starts at 0 */
        /* t = (-rev_pref_mod) mod n  ->  t = (n - rev_pref_mod) % n */
        seq_t t = n_mod - rev_pref_mod;
        if (t == n_mod) t = 0;            /* handle rev_pref_mod == 0 case */

        /* if t < 10^d, we found the minimal multiple */
        if (t < pow10) {
            return rev * pow10 + t;
        }

        /* Prepare for next d (increment number of appended zeros) */
        rev_pref_mod = (rev_pref_mod * (seq_t)10) % n_mod; /* rev * 10^(d+1) mod n */
        pow10 *= (seq_t)10;                                 /* 10^(d+1) as seq_t */
    }
}
