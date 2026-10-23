/*
 * A033008  Every run of digits of n in base 10 has length 2.
 * https://oeis.org/A033008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 11 * A043314(n) (= 11*n for n<10)
 * Range:     u8 n <= 9, u16 n <= 90, u32 n <= 800, u64 n <= 800,
 *            u128 n <= 800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence consists of all numbers whose decimal digits are
       grouped into runs of exactly two identical digits.  Each
       such number can be obtained by taking the decimal digits of an
       integer x (with no consecutive equal digits and first digit
       non‑zero) and writing each digit twice.  Computation proceeds in
       three stages: 1) find the length of the digit string of x,
       2) reconstruct the digit string of x from the index, 3) expand
       that string to the desired number by inserting a second copy
       of each digit.  */

    /* ---- 1.  Locate length of the compressed integer x ---- */
    idx_t idx = n - (idx_t)1;      /* change to a zero‑based index */
    /* Precompute powers of 9 up to a safe bound.  9^k counts the
       number of k‑digit strings with the required property.  */
    static unsigned __int128 pow9[64];
    static int pow9_ready = 0;
    if (!pow9_ready) {
        pow9[0] = (unsigned __int128)1;
        for (int i = 1; i < 64; ++i) {
            pow9[i] = pow9[i-1] * (unsigned __int128)9;
        }
        pow9_ready = 1;
    }

    unsigned __int128 cumulative = 0;
    idx_t length = 0;              /* number of digits of x */
    idx_t offset = 0;              /* index within this block */
    for (idx_t len = 1; ; ++len) {
        unsigned __int128 count = pow9[len];
        if (cumulative + count > (unsigned __int128)idx) {
            length = len;
            offset = (idx_t)((unsigned __int128)idx - cumulative);
            break;
        }
        cumulative += count;
    }

    /* ---- 2.  Reconstruct the digit string of x ---- */
    int digits[64];                /* at most 64 digits is ample */
    int prev = -1;                 /* previous digit (none yet) */
    for (idx_t pos = 0; pos < length; ++pos) {
        unsigned __int128 rem = pow9[(int)(length - 1 - pos)];   /* 9^(remaining) */
        /* Enumerate allowed digits in ascending order. */
        for (int cand = 0; cand <= 9; ++cand) {
            if (pos == 0 && cand == 0)          /* leading zero forbidden */
                continue;
            if (pos > 0 && cand == prev)       /* consecutive equal forbidden */
                continue;
            if ((idx_t)rem <= offset) {           /* skip this block */
                offset -= (idx_t)rem;
                continue;
            }
            digits[(int)pos] = cand;
            prev = cand;
            break;
        }
    }

    /* ---- 3.  Expand the digit string to produce the final value ---- */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < length; ++i) {
        /* result = result * 100 + digit * 11  (two copies of the digit) */
        result = result * (seq_t)100 + (seq_t)digits[(int)i] * (seq_t)11;
    }
    return result;
}
