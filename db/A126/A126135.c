/*
 * A126135  Take the base-3 representation of n, render that in decimal
 *          notation and take the base-3 representation of n again.
 * https://oeis.org/A126135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126135:
   a(n) = decimal value of the base‑3 representation of the
   decimal interpretation of the base‑3 representation of n.
   That is, write n in base‑3, read that string as a decimal
   number m, then write m in base‑3 and read that string as
   a decimal number again. */
seq_t a(idx_t n)
{
    /* First convert n → base‑3 digits and build the decimal number m */
    unsigned long long m = 0, p10 = 1;
    unsigned long long tmp = n;
    while (tmp > 0) {
        unsigned d = tmp % 3;
        m += d * p10;
        p10 *= 10;
        tmp /= 3;
    }
    /* For n == 0, m remains 0 already. */

    /* Now convert m → base‑3 digits and build the final decimal value a */
    unsigned long long a_val = 0;
    p10 = 1;
    unsigned long long t = m;
    if (t == 0) {
        a_val = 0;
    } else {
        while (t > 0) {
            unsigned d = t % 3;
            a_val += d * p10;
            p10 *= 10;
            t /= 3;
        }
    }

    return (seq_t) a_val;
}
