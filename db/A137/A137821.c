/*
 * A137821  Numbers k such that Sum_{j=1..2k} Catalan(j) == 0 (mod 3).
 * https://oeis.org/A137821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 499, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer k (k ≥ 1) such that
   Sum_{j=1..2k} Catalan(j) ≡ 0 (mod 3).  */
 
static const int _small_bin[3][3] = {
    {1, 0, 0},   /* C(0,0) */
    {1, 1, 0},   /* C(1,0)=1, C(1,1)=1 */
    {1, 2, 1}    /* C(2,0)=1, C(2,1)=2, C(2,2)=1 */
};
 
/* compute C(n,k) mod 3 using Lucas's theorem (prime modulus 3) */
static int _binom_mod3(idx_t n, idx_t k)
{
    if (k > n) return 0;
    int res = 1;
    while (n > 0 || k > 0) {
        int ni = n % 3;
        int ki = k % 3;
        if (ki > ni) return 0;
        res = (res * _small_bin[ni][ki]) % 3;
        n /= 3;
        k /= 3;
    }
    return res;   /* 0,1, or 2 */
}
 
/* Catalan(n) mod 3 = binom(2n,n) - binom(2n,n+1) (mod 3) */
static int _catalan_mod3(idx_t n)
{
    idx_t twice = 2 * n;
    int b1 = _binom_mod3(twice, n);
    int b2 = _binom_mod3(twice, n + 1);
    int r = b1 - b2;
    r %= 3;
    if (r < 0) r += 3;
    return r; /* 0,1,2 */
}
 
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; value for n=0 not defined */
    idx_t count = 0;
    idx_t k = 0;
    int sum_mod3 = 0;   /* current S(k) mod 3 */
 
    while (count < n) {
        ++k;
        int c1 = _catalan_mod3(2 * k - 1);
        int c2 = _catalan_mod3(2 * k);
        sum_mod3 += c1 + c2;
        sum_mod3 %= 3;
        if (sum_mod3 == 0) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
    return (seq_t)k;  /* should never be reached */
}
