/*
 * A060145  a(n) = floor(n/tau) - floor(n/(1 + tau)).
 * https://oeis.org/A060145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/τ) - floor(n/(1 + τ)) * With τ = (1+√5)/2. Using
 *            the identity * 1/τ + 1/τ² = 1 * we rewrite a(n) as * a(n) =
 *            2*floor(n/τ) - n + 1 * The value floor(n/τ) is obtained by the
 *            convergent * 1/τ ≈ Fk / Fk+1 (Fk = Fibonacci numbers) * with k
 *            chosen so that Fk+1 > √max(idx). For 64‑bit ind...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned long long FIB[50] = {
    0ULL, 1ULL, 1ULL, 2ULL, 3ULL, 5ULL, 8ULL, 13ULL, 21ULL, 34ULL,
    55ULL, 89ULL, 144ULL, 233ULL, 377ULL, 610ULL, 987ULL, 1597ULL,
    2584ULL, 4181ULL, 6765ULL, 10946ULL, 17711ULL, 28657ULL, 46368ULL,
    75025ULL, 121393ULL, 196418ULL, 317811ULL, 514229ULL, 832040ULL,
    1346269ULL, 2178309ULL, 3524578ULL, 5702887ULL, 9227465ULL,
    14930352ULL, 24157817ULL, 39088169ULL, 63245986ULL, 102334155ULL,
    165580141ULL, 267914296ULL, 433494437ULL, 701408733ULL,
    1134903170ULL, 1836311903ULL, 2971215073ULL, 4807526976ULL,
    7778742049ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* use convergent with k = 48 :  F48/F49  */
    const unsigned long long num = FIB[48];
    const unsigned long long den = FIB[49];

    /* floor(n/τ) = floor((n * F48) / F49)  */
    unsigned __int128 prod = (unsigned __int128)n * num;
    unsigned long long floor_val = prod / den;

    /* a(n) = 2*floor_val - n + 1 */
    unsigned __int128 result = (unsigned __int128)2 * floor_val - n + 1;
    return (seq_t)result;
}
