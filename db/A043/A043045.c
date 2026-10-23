/*
 * A043045  a(n)=(s(n)+2)/3, where s(n)=n-th base 3 palindrome that starts
 *          with 1.
 * https://oeis.org/A043045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = (s(n)+2)/3 where s(n) is the n‑th
 * base‑3 palindrome that starts with 1.  The palindromes
 * are generated in increasing order of their numeric
 * value.  The function works for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    idx_t rem = n - 1;              /* offset within the current length group */

    /* Find the length L of the palindrome containing the n‑th one.
       Since the length grows slowly (about 2 digits per power of 3),
       a simple linear search is efficient.  Count(L) = 3^(ceil(L/2)-1). */
    idx_t L = 1;
    while (1) {
        idx_t m   = (L + 1) / 2;    /* ceil(L/2) */
        idx_t cnt = 1;
        for (idx_t i = 0; i < m - 1; ++i) cnt *= 3; /* cnt = 3^(m-1) */
        if (rem < cnt) break;
        rem -= cnt;
        ++L;
    }

    /* Now rem is the index (0‑based) within the palindromes of length L. */
    idx_t m      = (L + 1) / 2;
    unsigned char d[20];           /* first half digits, starting with 1; 20 is safe */
    d[0] = 1;

    /* Compute the remaining digits of the first half from base‑3 representation of rem. */
    for (idx_t i = 2; i <= m; ++i) {
        idx_t power = 1;
        for (idx_t j = 0; j < m - i; ++j) power *= 3;  /* power = 3^(m-i) */
        d[i - 1] = (unsigned char)((rem / power) % 3);
    }

    /* Build the numeric value of the palindrome in base 3. */
    seq_t val = 0;
    for (idx_t i = 0; i < m; ++i) val = val * 3 + d[i];
    for (idx_t i = 0; i < L - m; ++i) {
        idx_t idx = (L % 2 == 0) ? (m - 1 - i) : (m - 2 - i);
        val = val * 3 + d[idx];
    }

    /* Convert to the final sequence value.  val ≡ 1 (mod 3), so (val+2)/3 is integer. */
    return (val + 2) / 3;
}
