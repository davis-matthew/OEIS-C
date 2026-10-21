/*
 * A083982  Smallest palindromic prime containing the digit string of n.
 * https://oeis.org/A083982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest palindromic prime that contains the decimal
 *            representation of n as a contiguous substring. The algorithm
 *            enumerates palindromes of increasing length, checks for the
 *            substring and primality. It is efficient for n up to about 10
 *            000, and works for any seq_t and idx_t types defined b...
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Powers of ten up to 10^10, enough for all prefixes needed here. */
static const uint64_t POW10[] = {
    1ULL,         10ULL,       100ULL,      1000ULL,    10000ULL,
    100000ULL,    1000000ULL,  10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL
};

/* Convert an unsigned 64‑bit integer to a decimal string.  Result is written to buf
   and its length is returned.  The buffer must have space for at least 21 bytes. */
static size_t u64_to_str(uint64_t v, char *buf) {
    char tmp[32];
    size_t i = 0;
    if (v == 0) {
        tmp[i++] = '0';
    } else {
        while (v > 0) {
            tmp[i++] = '0' + (v % 10);
            v /= 10;
        }
    }
    size_t len = i;
    for (size_t j = 0; j < i / 2; ++j) {
        char c = tmp[j];
        tmp[j] = tmp[i - 1 - j];
        tmp[i - 1 - j] = c;
    }
    memcpy(buf, tmp, len);
    return len;
}

/* Helper: modular multiplication using 128‑bit arithmetic to avoid overflow. */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* Helper: modular exponentiation. */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    /* Small primes check */
    static const uint64_t small_primes[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Write n-1 as d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Convert the index to its decimal string representation. */
    char nbuf[32];
    size_t nlen = u64_to_str((uint64_t)n, nbuf);

    /* Search palindromic primes of increasing length.  Upper bound of 13
       digits is sufficient for all n up to 10 000. */
    for (int L = (int)nlen; L <= 13; ++L) {
        int half = (L + 1) / 2;
        uint64_t start = POW10[half - 1];
        uint64_t end   = POW10[half] - 1;

        for (uint64_t pref = start; pref <= end; ++pref) {
            /* Build the palindrome from the prefix. */
            uint64_t pal = pref;
            uint64_t tmp = pref;
            if (L & 1) tmp /= 10;          /* skip middle digit for odd length */
            while (tmp > 0) {
                pal = pal * 10 + (tmp % 10);
                tmp /= 10;
            }

            /* Convert the palindrome to a string. */
            char pbuf[32];
            size_t plen = u64_to_str(pal, pbuf);

            /* Check whether the decimal string of n is a substring. */
            bool contains = false;
            for (size_t pos = 0; pos + nlen <= plen; ++pos) {
                if (memcmp(pbuf + pos, nbuf, nlen) == 0) {
                    contains = true;
                    break;
                }
            }
            if (!contains) continue;

            /* Prime test. */
            if (isPrime64(pal)) {
                return (seq_t)pal;
            }
        }
    }
    /* Fallback: should never occur for the required input range. */
    return (seq_t)0;
}
