/*
 * A085006  Let S(0)={1,1,2} S(n)={S(n-1), S(n-1)-{x},{3-x}} where x is the
 *          last element of S(n-1), then sequence is S(infinity).
 * https://oeis.org/A085006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085006 : Fractal sequence
    a(1)=1, a(2)=1, a(3)=2
    For n > 3:
        Let Lk = 3 * 2^k be the smallest length ≥ n.
        If n == Lk      →  a(n) = 1  if k is odd, else 2
        else (n > Lk/2) →  a(n) = a(n – Lk/2)
    This follows directly from the definition  
    S(0) = {1,1,2}
    S(k) = S(k‑1) , S(k‑1) without its last element , {3 – last(S(k‑1))}
*/
seq_t a(idx_t n)
{
    /* base values for n = 1, 2, 3 */
    static const seq_t base[3] = {(seq_t)1, (seq_t)1, (seq_t)2};

    if (n <= (idx_t)3)
        return base[n - 1];

    idx_t len = (idx_t)3;   /* current stage length Lk = 3 * 2^k   */
    idx_t k   = 0;          /* exponent k (so len = 3 << k)     */

    /* grow len until it covers n */
    while (len < n) {
        len <<= 1;   /* len = len * 2  (2^k grows) */
        k++;
    }

    /* if n is the very last element of this stage, return 3‑last(prev) */
    if (n == len)
        return (k & 1) ? (seq_t)1 : (seq_t)2;  /* use k parity */

    /* otherwise n lies in the second half: map back one level */
    return a(n - (len >> 1));
}
