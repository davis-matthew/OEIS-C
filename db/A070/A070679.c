/*
 * A070679  Smallest m in range 1..phi(n) such that 9^m == 1 mod n, or 0 if no
 *          such number exists.
 * https://oeis.org/A070679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m (1 ≤ m ≤ φ(n)) such that 9^m ≡ 1 (mod n), or 0
 *            if no such m exists. Since 9 ≡ 0 (mod 3), the condition fails
 *            for any n divisible by 3. For the remaining n we compute the
 *            multiplicative order of 9 modulo n, which always divides φ(n).
 *            The order is found by testing all prime diviso...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: modular exponentiation (base^exp mod mod) using 128‑bit
   intermediate to avoid overflow.  */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 0 by definition */
        return 0;

    if (n % 3 == 0)             /* 9 is 0 modulo 3, so never 1 */
        return 0;

    /* Compute φ(n) */
    uint64_t nn   = (uint64_t)n;
    uint64_t phi  = nn;
    uint64_t temp = nn;

    /* factor n to compute φ(n) */
    for (uint64_t p = 2; p * p <= temp; ++p)
        if (temp % p == 0)
        {
            phi -= phi / p;
            while (temp % p == 0)
                temp /= p;
        }
    if (temp > 1)
        phi -= phi / temp;

    /* Factor φ(n) to find its distinct prime divisors */
    uint64_t phi_tmp = phi;
    uint64_t pf[16];      /* φ(n) ≤ 10 000 for the intended range */
    int pf_cnt = 0;
    for (uint64_t p = 2; p * p <= phi_tmp; ++p)
        if (phi_tmp % p == 0)
        {
            pf[pf_cnt++] = p;
            while (phi_tmp % p == 0)
                phi_tmp /= p;
        }
    if (phi_tmp > 1)
        pf[pf_cnt++] = phi_tmp;

    /* Compute the minimal order of 9 modulo n */
    uint64_t order = phi;
    for (int i = 0; i < pf_cnt; ++i)
    {
        uint64_t q = pf[i];
        while (order % q == 0)
        {
            uint64_t cand = order / q;
            if (mod_pow64(9, cand, nn) == 1)
                order = cand;
            else
                break;
        }
    }
    return (seq_t)order;
}
