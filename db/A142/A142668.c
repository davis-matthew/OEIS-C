/*
 * A142668  Primes congruent to 5 mod 57.
 * https://oeis.org/A142668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: nth prime ≡ 5 (mod 57) found by trial division.
   The sequence is defined by a(n) = the n-th prime p such that p ≡ 5 (mod 57). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* primes up to 1000 – sufficient for all candidates up to ≈ 1 000 000. */
static const uint32_t small_primes[] = {
    2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U, 31U, 37U, 41U, 43U, 47U,
    53U, 59U, 61U, 67U, 71U, 73U, 79U, 83U, 89U, 97U, 101U, 103U, 107U,
    109U, 113U, 127U, 131U, 137U, 139U, 149U, 151U, 157U, 163U, 167U, 173U,
    179U, 181U, 191U, 193U, 197U, 199U, 211U, 223U, 227U, 229U, 233U, 239U,
    241U, 251U, 257U, 263U, 269U, 271U, 277U, 281U, 283U, 293U, 307U, 311U,
    313U, 317U, 331U, 337U, 347U, 349U, 353U, 359U, 367U, 373U, 379U, 383U,
    389U, 397U, 401U, 409U, 419U, 421U, 431U, 433U, 439U, 443U, 449U, 457U,
    461U, 463U, 467U, 479U, 487U, 491U, 499U, 503U, 509U, 521U, 523U, 541U,
    547U, 557U, 563U, 569U, 571U, 577U, 587U, 593U, 599U, 601U, 607U,
    613U, 617U, 619U, 631U, 641U, 643U, 647U, 653U, 659U, 661U, 673U, 677U,
    683U, 691U, 701U, 709U, 719U, 727U, 733U, 739U, 743U, 751U, 757U, 761U,
    769U, 773U, 787U, 797U, 809U, 811U, 821U, 823U, 827U, 829U, 839U, 853U,
    857U, 859U, 863U, 877U, 881U, 883U, 887U, 907U, 911U, 919U, 929U, 937U,
    941U, 947U, 953U, 967U, 971U, 977U, 983U, 991U, 997U
};
static const size_t small_primes_count = sizeof small_primes / sizeof small_primes[0];

/* trial‑division primality test using the small primes table. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2U) return false;
    for (size_t i = 0; i < small_primes_count; ++i) {
        uint32_t p = small_primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) return n == (uint64_t)p;
    }
    return true;
}

/* ---------------------------------------------------------- */
/*  Sequence entry function                                    */
/* ---------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) /* undefined for offset 1, but return 0 safely */
        return 0;

    idx_t found = 0;            /* number of primes found so far */
    uint64_t m = 0;             /* progression index */

    while (found < n) {
        uint64_t candidate = 5ULL + 57ULL * m;   /* p ≡ 5 (mod 57) */
        if (is_prime_uint64(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++m;
    }
    /* unreachable, but keep the compiler happy */
    return 0;
}
