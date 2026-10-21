/*
 * A053024  a(n) = n*p where p is the next prime >= n.
 * https://oeis.org/A053024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * p, where p is the smallest prime ≥ n. The routine
 *            uses a simple deterministic trial‑division primality test
 *            (sufficient for n up to several million; for larger n the
 *            algorithm still works but slower). The product is computed in
 *            128‑bit intermediate to avoid overflow when seq_t is 64 or...
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- primality test (trial division) ---------- */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5, w = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += w;      /* alternate 2,4 steps (i=5,7,11,13,...) */
        w = 6 - w;
    }
    return true;
}

/* ---------- smallest prime >= n ---------- */
static uint64_t next_prime_u64(uint64_t n) {
    if (n <= 2) return 2;
    if (n % 2 == 0) n++;           /* make odd */
    for ( ; ; n += 2) {
        if (is_prime_u64(n)) return n;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;          /* input index, 64‑bit */
    uint64_t p = next_prime_u64(N);    /* next prime ≥ N */
    unsigned __int128 prod = (__uint128_t)N * (__uint128_t)p;
    return (seq_t)prod;                /* truncate if seq_t smaller */
}
