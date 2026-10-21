/*
 * A068485  One-sixtieth of the even leg of Pythagorean triangles whose other
 *          sides are both primes (other than 3, 5 or 13).
 * https://oeis.org/A068485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k(k+1)/30 where k is the n-th integer ≠ 1 such that both
 *            2k+1 and 2k(k+1)+1 are prime (i.e., the even leg of a primitive
 *            Pythagorean triangle whose two odd legs are primes)
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline int is_prime_u64(uint64_t n) {
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return 1;
        if (n % p == 0) return 0;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        __int128 x = 1, base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        uint64_t xx = (uint64_t)x;
        if (xx == 1ULL || xx == n-1) continue;
        int cont = 0;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            xx = (uint64_t)x;
            if (xx == n-1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

static seq_t *terms = NULL;
static size_t terms_len = 0, terms_cap = 0;
static uint64_t next_k = 1;   /* next k to test */

static inline void ensure_terms_cap(size_t needed) {
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (terms_len < (size_t)n) {
        uint64_t k = next_k++;
        if (!is_prime_u64(2ULL * k + 1ULL))
            continue;
        unsigned __int128 prod = (unsigned __int128)k * (k + 1ULL);
        uint64_t q = (uint64_t)((__int128)2ULL * prod + 1ULL);
        if (!is_prime_u64(q))
            continue;
        /* prod is always divisible by 30 for valid k */
        if ((prod % 30) != 0) continue;
        seq_t val = (seq_t)(prod / 30ULL);
        ensure_terms_cap(terms_len + 1);
        terms[terms_len++] = val;
    }
    return terms[n - 1];
}
