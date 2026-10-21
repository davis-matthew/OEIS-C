/*
 * A213060  Lucas(n) mod n, where Lucas = A000032.
 * https://oeis.org/A213060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) mod n Lucas numbers: L(0)=2, L(1)=1,
 *            L(k)=L(k-1)+L(k-2) a(n) need only the value L(n) modulo n,
 *            computed iteratively
 * Range:     u8 n <= 208, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* For n==1: L(1)=1, 1 mod 1 = 0. */
        return 0;
    }

    /* Reduce operations modulo n: use a local seq_t for the modulus */
    const seq_t mod = (seq_t)n;

    /* Lucas(0) and Lucas(1) reduced modulo n */
    seq_t L0 = 2;
    if (L0 >= mod) L0 -= mod;          /* L0 = 2 mod n */
    seq_t L1 = 1;                      /* L1 = 1 mod n */

    /* Iteratively compute Lucas numbers up to L(n) modulo n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = L1 + L0;          /* L(i) = L(i-1)+L(i-2) */
        if (next >= mod) next -= mod;  /* reduce modulo n efficiently */
        L0 = L1;
        L1 = next;
    }

    return L1; /* L(n) modulo n */
}
