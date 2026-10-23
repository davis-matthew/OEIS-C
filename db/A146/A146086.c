/*
 * A146086  Number of n-digit numbers with each digit odd where the digits 1
 *          and 3 occur an even number of times.
 * https://oeis.org/A146086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (5^n + 2*3^n + 1)/4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (valid for n ≥ 4): a(1)=3, a(2)=11, a(3)=45; 
   a(n) = 9*a(n-1) - 23*a(n-2) + 15*a(n-3) */

seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly. */
    if (n == 0) return 0;           /* not used but safe */
    if (n == 1) return 3;
    if (n == 2) return 11;
    if (n == 3) return 45;

    /* Iteratively compute terms up to n using the recurrence. */
    seq_t a1 = 3;   /* a(1) */
    seq_t a2 = 11;  /* a(2) */
    seq_t a3 = 45;  /* a(3) */
    seq_t an;

    for (idx_t k = 4; k <= n; ++k) {
        an = a3 * 9 - a2 * 23 + a1 * 15;  /* a(k) = 9*a(k‑1) – 23*a(k‑2) + 15*a(k‑3) */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
