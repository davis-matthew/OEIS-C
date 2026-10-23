/*
 * A029954  Palindromic in base 7.
 * https://oeis.org/A029954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 533, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029954: Palindromic in base 7.
   a(n) is the n-th nonnegative integer whose base‑7 representation is a palindrome. */
 
seq_t a(idx_t n)
{
    /* Handle the first 7 one‑digit palindromes (0 … 6). */
    if (n <= 7) {
        return (seq_t)(n - 1);
    }

    /* Find the length L of the palindrome that contains the n‑th term. */
    idx_t L = 2;
    seq_t cum = 7;               /* number of palindromes of length 1 */

    while (1) {
        /* Count of length‑L palindromes: 6 * 7^(ceil(L/2)-1) */
        unsigned half = (L + 1) / 2;
        seq_t cnt = 6;           /* leading digit cannot be zero */
        for (unsigned i = 1; i < half; ++i) {
            cnt *= 7;
        }
        if (cum + cnt >= n) {
            break;
        }
        cum += cnt;
        ++L;
    }

    unsigned half = (unsigned)((L + 1) / 2);

    /* Zero‑based offset within the block of length‑L palindromes. */
    seq_t offset = (seq_t)n - 1 - cum;

    /* Compute the digits of the first half of the palindrome. */
    unsigned first_half[64] = {0};  /* enough for all practical lengths */
    seq_t pow = 1;
    for (unsigned i = 1; i < half; ++i) {
        pow *= 7;                   /* pow = 7^(half-1) */
    }

    seq_t first_idx = offset / pow;  /* leading digit index: 0 … 5 */
    seq_t rem = offset % pow;
    first_half[0] = (unsigned)(first_idx + 1);  /* leading digit 1 … 6 */

    for (unsigned j = 1; j < half; ++j) {
        pow /= 7;                         /* next lower weight */
        first_half[j] = (unsigned)(rem / pow);
        rem %= pow;
    }

    /* Assemble the full palindrome in base 7. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        seq_t d;
        if ((unsigned)i < half) {
            d = first_half[i];
        } else {
            d = first_half[L - 1 - i];
        }
        val = val * 7 + d;
    }

    return val;
}
