/*
 * A021582  Decimal expansion of 1/578.
 * https://oeis.org/A021582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 578) % 10 This is the (n)-th digit after
 *            the decimal point of 1/578
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of sequence A021582.  */
seq_t a(idx_t n) {
    /* We need the quotient floor(10^(n+1)/578) modulo 10.
       Compute 10^(n+1) modulo (578*10)=5780.  Then
       floor((10^(n+1) mod 5780) / 578) yields the desired digit.  */
    idx_t exp = n + 1ULL;           /* exponent (n+1) */
    uint64_t mod = 5780ULL;         /* 578 * 10  */
    uint64_t base = 10ULL;          /* base 10  */
    uint64_t result = 1ULL;         /* result of modular exponentiation */

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }

    /* result now holds 10^(n+1) mod 5780.
       Divide by 578 to get the quotient modulo 10.  */
    seq_t digit = (seq_t)(result / 578ULL);
    return digit;
}
