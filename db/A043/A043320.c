/*
 * A043320  Numbers which, written in base 256, have all digits less than 16
 *          and no two adjacent digits equal.
 * https://oeis.org/A043320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 240, u32 n <= 1800, u64 n <= 1800,
 *            u128 n <= 1800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A043320  
    Numbers whose representation in base 256 uses only the digits
    0–15 and has no two adjacent equal digits.
    The function builds the n‑th term by determining the length k of
    the base‑256 representation, then decoding the offset within the
    k‑digit blocks using a mixed‑radix system with radix 15 at every
    position (the most significant digit is restricted to 1–15). */
seq_t a(idx_t n)
{
    /* find the number of base‑256 digits k such that
       sum_{i=1}^{k-1} 15^i < n <= sum_{i=1}^{k} 15^i        */
    unsigned __int128 total = 0;           /* cumulative count of shorter lengths */
    unsigned __int128 block  = 15;         /* 15^1  */
    int k = 1;
    unsigned __int128 target = (unsigned __int128)n;

    while (total + block < target) {
        total += block;
        block *= 15;
        ++k;
    }

    /* offset of n within the block of k‑digit numbers (0‑based) */
    unsigned __int128 offset = target - total - 1;

    /* multiplier for the highest position: 15^(k‑1) */
    unsigned __int128 mult = 1;
    for (int i = 1; i < k; ++i)
        mult *= 15;

    seq_t value = 0;
    unsigned int prev = 16;   /* sentinel – not a valid digit */
    int remaining = k;
    while (remaining > 0) {
        /* index of the chosen digit among the allowed digits */
        unsigned int idx = (unsigned int)(offset / mult);

        /* select the idx‑th allowed digit in ascending order */
        unsigned int digit = 0;
        unsigned int count = 0;
        for (int cand = 0; cand < 16; ++cand) {
            if (cand == prev)                    continue;          /* no adjacency */
            if (remaining == k && cand == 0)     continue;          /* leading zero forbidden */
            if (count == idx) { digit = cand; break; }
            ++count;
        }

        /* update offset and the constructed value */
        offset %= mult;
        value = value * 256 + (seq_t)digit;

        prev = digit;
        --remaining;
        if (mult > 1)           /* divide by 15 for the next lower position */
            mult /= 15;
    }

    return value;
}
