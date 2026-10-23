/*
 * A043006  Base-5 palindromes that start with 1.
 * https://oeis.org/A043006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base‑5 palindromes that start with 1, in increasing order
 * (OEIS A043006).  The function generates the nth such number for any
 * n ≥ 1 without using a lookup table.  It works for any seq_t that can
 * handle the result; intermediate calculations are performed with
 * unsigned 64‑bit arithmetic when possible. */
seq_t a(idx_t n)
{
    /* Precompute powers of 5 up to a safe limit (~5³⁰).   */
    static const idx_t pow5[32] = {
        1ULL,
        5ULL,
        25ULL,
        125ULL,
        625ULL,
        3125ULL,
        15625ULL,
        78125ULL,
        390625ULL,
        1953125ULL,
        9765625ULL,
        48828125ULL,
        244140625ULL,
        1220703125ULL,
        6103515625ULL,
        30517578125ULL,
        152587890625ULL,
        762939453125ULL,
        3814697265625ULL,
        19073486328125ULL,
        95367431640625ULL,
        476837158203125ULL,
        2384185791015625ULL,
        11920928955078125ULL,
        59604644775390625ULL,
        298023223876953125ULL,
        1490116119384765625ULL,
        7450580596923828125ULL,
        37252902984619140625ULL,
        186264514923095703125ULL
    };

    /* Find the length L of the palindrome that contains the nth term. */
    idx_t cumulative = 0ULL;
    int L = 1;
    for (;; ++L) {
        idx_t count;
        if (L == 1 || L == 2) {
            count = 1ULL;
        } else if (L % 2 == 1) {                  /* odd length 2k+1   */
            int k = (L - 1) / 2;                  /* half of the free digits */
            count = pow5[k];
        } else {                                  /* even length 2k    */
            int k = L / 2;
            count = pow5[k - 1];
        }
        if (n <= cumulative + count) {
            /* offset within this block (0‑based) */
            idx_t offset = n - cumulative - 1ULL;

            /* number of free digits (apart from the leading 1) */
            int half = (L + 1) / 2;          /* first half length   */
            int len  = half - 1;             /* free digits in the half */
            int freeDigits[32] = {0};
            idx_t tmp = offset;
            for (int i = len - 1; i >= 0; --i) {
                freeDigits[i] = (int)(tmp % 5ULL);
                tmp /= 5ULL;
            }

            /* Build the full digit array on the fly and convert to value */
            seq_t val = 0;
            for (int i = 0; i < L; ++i) {
                int d;
                if (i < half) {            /* in the first half */
                    if (i == 0)
                        d = 1;
                    else
                        d = freeDigits[i - 1];
                } else {                    /* mirrored part   */
                    d = (i == L - 1) ? 1 : freeDigits[L - 1 - i - 1];
                }
                val = val * 5 + d;
            }
            return val;
        }
        cumulative += count;
    }
}
