/*
 * A129918  Smallest prime of the form k*prime(n+1)+prime(n) =
 *          j*prime(n+2)+prime(n+1) for free integer multipliers k and j.
 * https://oeis.org/A129918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 20, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129918:  smallest prime p for which
 *  p ≡ prime(n)   (mod prime(n+1))
 *  p ≡ prime(n+1) (mod prime(n+2))
 *
 *  For index n (1‑based) we solve the two congruences by the Chinese
 *  remainder theorem, obtain the least non‑negative solution x, and
 *  then search the arithmetic progression x + k·M (M = prime(n+1)·prime(n+2))
 *  for the first prime.  The search uses the arbitrary‑precision primality
 *  test is_prime_big.  The function works for any seq_t size permitted by
 *  the build and keeps all intermediate arithmetic in seq_t or 64‑bit
 *  unsigned integers only. */

/* --------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

#if defined(__GNUC__) || defined(__clang__)
#  define INT128 signed __int128
#  define UINT128 unsigned __int128
#else
#  error "Compiler does not support 128‑bit integer type."
#endif

/* --------------------------------------------------------------------- */
/*  Extended Euclidean algorithm to compute the modular inverse of a
 *  modulo m.  Both a and m fit in 64 bits and are coprime (they are
 *  consecutive primes, hence coprime).  The result is in [0, m‑1].  */
static inline uint64_t modinv_u64(uint64_t a, uint64_t m)
{
    INT128 t    = 0;
    INT128 newt = 1;
    INT128 r    = (INT128)m;
    INT128 newr = (INT128)a;

    while (newr != 0) {
        uint64_t q     = (uint64_t)(r / newr);
        INT128 left    = t    - (INT128)q * newt;
        t              = newt;
        newt           = left;
        left            = r    - (INT128)q * newr;
        r              = newr;
        newr           = left;
    }
    if (r != 1) return 0;           /* inverse does not exist – should
                                        not happen for prime moduli */
    if (t < 0) t += m;
    return (uint64_t)t;
}

/* --------------------------------------------------------------------- */
/*  Compute a(n) for a single 1‑based index n.  The function is safe
 *  for any n >= 1.  If n < 1 the function returns 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined; safe fallback */

    /* 1. Get the three consecutive primes. */
    uint32_t p  = get_prime(n);     /* prime(n)   */
    uint32_t p1 = get_prime(n + 1); /* prime(n+1) */
    uint32_t p2 = get_prime(n + 2); /* prime(n+2) */

    /* 2. Compute modular inverse of p1 modulo p2 and the CRT offset. */
    uint64_t inv  = modinv_u64(p1, p2);   /* inverse of p1 modulo p2 */

    /* Difference r2 - r1 mod p2 (positive). */
    int64_t diff = (int64_t)p1 - (int64_t)p;   /* p1 - p */
    diff %= (int64_t)p2;
    if (diff < 0) diff += p2;

    uint64_t t = (uint64_t)((UINT128)diff * inv % p2);  /* CRT coefficient */

    /* Least non‑negative solution of the two congruences. */
    seq_t x = (seq_t)p + (seq_t)p1 * (seq_t)t;
    seq_t M = (seq_t)p1 * (seq_t)p2;        /* modulus of the progression */
    x %= M;
    if (x <= (seq_t)1) x += M;              /* avoid 0 or 1 */

    /* 3. Search the arithmetic progression x + k·M for the first prime. */
    seq_t candidate = x;
    while (!is_prime_big(candidate))
        candidate += M;

    return candidate;
}
