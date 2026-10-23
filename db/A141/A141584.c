/*
 * A141584  a(n) = the closest multiple of A053669(2n) to 2n. A053669(2n) =
 *          the smallest prime coprime to 2n.
 * https://oeis.org/A141584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the closest multiple of the smallest prime coprime * to
 *            2n to 2n. * Let m = 2n. Find the smallest prime p with
 *            gcd(m,p)=1. * Then a(n) = argmin_{k∈ℕ} |k·p−m|. (Because p is
 *            odd, the tie * cannot occur.)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for 64‑bit integers */
static bool is_prime_uint(uint64_t x)
{
    if (x < 2u) return false;
    if (x == 2u) return true;
    if ((x & 1u) == 0u) return false;
    for (uint64_t d = 3u; d <= x / d; d += 2u)
        if (x % d == 0u) return false;
    return true;
}

/* Compute a(n) as defined above */
seq_t a(idx_t n)
{
    /* m = 2·n  (n is unsigned 64‑bit) */
    uint64_t m = (uint64_t)2u * (uint64_t)n;

    /* Find the smallest odd prime not dividing m */
    uint64_t p = 3u;
    while (true) {
        if ((m % p) != 0u && is_prime_uint(p))
            break;
        p += 2u;  /* skip even numbers – 2 can never be coprime to m */
    }

    /* Nearest multiples of p */
    uint64_t k = m / p;
    seq_t lower = ((seq_t)k) * ((seq_t)p);
    seq_t upper = ((seq_t)(k + 1u)) * ((seq_t)p);

    seq_t diff_lower = ((seq_t)m) - lower;
    seq_t diff_upper = upper - ((seq_t)m);

    return diff_lower <= diff_upper ? lower : upper;
}
