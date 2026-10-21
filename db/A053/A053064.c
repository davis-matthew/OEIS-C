/*
 * A053064  Alternately append n to end or beginning of previous term.
 * https://oeis.org/A053064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A053064: a(1)=1.
   For n>1: if n is even, a(n)=a(n‑1)*10^{digits(n)}+n;
             if n is odd, a(n)=n*10^{digits(a(n‑1))}+a(n‑1).
   The algorithm keeps the length (in decimal digits) of the current term
   in `len` and the value 10^len in `pow10_len`.  Each iteration
   recomputes 10^{digits(k)} as `pow10_dk`, uses it to update `a(n)`
   and then multiplies `pow10_len` by `pow10_dk` to reflect the new
   term length. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined below offset 1 */

    seq_t result = (seq_t)1;              /* a(1) */
    if (n == 1) return result;

    /* current term length (decimal digits) and 10^len */
    idx_t len = 1;
    seq_t pow10_len = (seq_t)10;          /* 10^len */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* number of decimal digits of k */
        idx_t dk = 0;
        idx_t temp = k;
        while (temp) { ++dk; temp /= 10; }

        /* 10^{dk} as seq_t */
        seq_t pow10_dk = (seq_t)1;
        for (idx_t i = 0; i < dk; ++i)
            pow10_dk *= (seq_t)10;

        if ((k & 1) == 0)  /* even k — append to the right */
        {
            result = result * pow10_dk + (seq_t)k;
        }
        else               /* odd k  — prepend to the left */
        {
            result = (seq_t)k * pow10_len + result;
        }

        len += dk;
        pow10_len = pow10_len * pow10_dk;  /* update 10^len for next step */
    }

    return result;
}
