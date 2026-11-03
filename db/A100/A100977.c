/*
 * A100977  Number of all extensions over Q_3 with degree n in the algebraic
 *          closure of Q_3.
 * https://oeis.org/A100977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{d|h} d) * (Sum_{s=0..m} ((3^(m+s+1)-3^(2*s))/2) *
 *            (3^(eps(s)*n)-3^(eps(s-1)*n))) ; where n = h*3^m, gcd(h,3)=1,
 *            eps(s)=Sum_{i=1..s} 1/3^i, eps(0)=0, eps(-1)=-∞
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Validated: standard, pass, 67 terms through n = 3824 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper: exponentiation by squaring for seq_t base and 64-bit exponent */
static seq_t pow_seq64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* helper: sum of divisors of an unsigned integer x (x >= 1) */
static seq_t sum_divisors(idx_t x)
{
    seq_t sum = 1;             /* include divisor 1 */
    idx_t i;
    for (i = 2; i * i <= x; ++i)
    {
        if (x % i == 0)
        {
            sum += i;
            idx_t other = x / i;
            if (other != i) sum += other;
        }
    }
    if (x > 1) sum += x;       /* include x itself */
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used, but safe */

    /* decompose n = h * 3^m with h coprime to 3 */
    idx_t temp = n, m = 0;
    while (temp % 3 == 0) { temp /= 3; ++m; }
    seq_t h = (seq_t)temp;     /* h > 0 */

    /* first factor: sum of divisors of h */
    seq_t sigma = sum_divisors(temp);

    /* second factor: sum over s = 0..m */
    seq_t total = 0;
    seq_t prev_pow = 1;        /* 3^0 */

    /* term for s = 0 */
    seq_t pow3_m_plus1 = pow_seq64(3, m + 1);
    seq_t factor0 = (pow3_m_plus1 - 1) / 2;  /* (3^(m+1)-1)/2 */
    total += factor0;

    for (idx_t s = 1; s <= m; ++s)
    {
        /* compute (3^(m+s+1)-3^(2*s))/2 */
        seq_t pow3_m_s_plus1 = pow_seq64(3, m + s + 1);
        seq_t pow3_2s = pow_seq64(3, 2 * s);
        seq_t factor = (pow3_m_s_plus1 - pow3_2s) / 2;

        /* exponent for 3^(eps(s)*n) = 3^((h*(3^m - 3^(m-s)))/2) */
        seq_t diff_pow = pow_seq64(3, m) - pow_seq64(3, m - s); /* 3^m - 3^(m-s), even */
        seq_t e_s = h * (diff_pow / 2);                         /* integer division by 2 */

        seq_t curr_pow = pow_seq64(3, (idx_t)e_s);

        seq_t diff = curr_pow - prev_pow;
        total += factor * diff;

        prev_pow = curr_pow;
    }

    return sigma * total;
}
