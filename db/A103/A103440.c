/*
 * A103440  a(n) = Sum[d|n, d==1 (mod 3), d^2] - Sum[d|n, d==2 (mod 3), d^2].
 * https://oeis.org/A103440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n, d == 1 (mod 3)} d^2 - Sum_{d|n, d == 2 (mod 3)}
 *            d^2
 * Range:     i8 n <= 12, i16 n <= 184, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t prime_power_value(idx_t p, idx_t exp)
{
    /* Returns a(p^exp) according to the recurrence:
       a(3^e) = 1
       a(p^e) = a(p) * a(p^(e-1)) - z * a(p^(e-2))
       where z = Kronecker(-3, p) * p^2,  Kronecker(-3, p) =
       0 if p == 3, 1 if p ≡ 1 (mod 3), -1 if p ≡ 2 (mod 3).  */
    if (exp == 0)
        return (seq_t)1;
    if (p == 3)
        return (seq_t)1;     /* a(3^e) = 1 for all e >= 1 */

    /* Compute Kronecker(-3, p) via simple congruence */
    int k = (p % 3 == 1) ? 1 : -1;      /* p != 3, so p mod 3 is 1 or 2 */
    seq_t z = (seq_t)k * (seq_t)p * (seq_t)p;   /* z = k * p^2 */

    seq_t a_p = z + 1;                  /* a(p) = z + 1 */
    if (exp == 1)
        return a_p;

    seq_t a_prevprev = (seq_t)1;        /* a(p^0) */
    seq_t a_prev = a_p;                 /* a(p^1) */
    seq_t a_curr;
    for (idx_t e = 2; e <= exp; ++e) {
        a_curr = a_p * a_prev - z * a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }
    return a_prev;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined for offset 1, but safe */
    if (n == 1)
        return (seq_t)1;

    idx_t temp = n;
    seq_t result = (seq_t)1;

    /* Factor out powers of 2 */
    if ((temp & 1) == 0) {
        idx_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++exp;
        }
        result *= prime_power_value(2, exp);
    }

    /* Factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= prime_power_value(p, exp);
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        result *= prime_power_value(temp, 1);
    }

    return result;
}
