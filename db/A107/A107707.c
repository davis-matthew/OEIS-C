/*
 * A107707  3-almost primes whose prime factors sum to a prime.
 * https://oeis.org/A107707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107707 – 3‑almost primes whose prime factors sum to a prime
   a(n) is the n‑th integer >1 such that Ω(n)=3 and the sum of its
   prime factors (with multiplicity) is prime.  The function generates
   terms by sieving smallest prime factors and testing the condition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers */
static bool isPrime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = 1, pow = d, base = a;
        while (pow > 0) {
            if (pow & 1ULL) x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            pow >>= 1ULL;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Smallest prime factor sieve */
static uint32_t *spf = NULL;
static uint32_t spf_limit = 0;

static void ensure_spf(uint32_t limit)
{
    if (limit <= spf_limit) return;
    uint32_t *new_spf = (uint32_t*)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf = new_spf;
    for (uint32_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* Dynamic storage of sequence terms */
static seq_t *seq_array = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static uint32_t last_checked = 1;   /* last integer examined */

static void push_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
        seq_t *new_arr = (seq_t*)realloc(seq_array, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        seq_array = new_arr;
        seq_cap = new_cap;
    }
    seq_array[seq_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1 */

    while (seq_len < (size_t)n) {
        ++last_checked;
        if (last_checked > spf_limit) {
            uint32_t new_limit = spf_limit == 0 ? 1024 : spf_limit * 2;
            if (new_limit < last_checked) new_limit = last_checked;
            ensure_spf(new_limit);
        }

        uint32_t temp = last_checked;
        int omega = 0;
        uint64_t sum = 0;
        while (temp > 1) {
            uint32_t p = spf[temp];
            if (p == 0) p = temp;        /* prime not yet in sieve */
            ++omega;
            sum += p;
            temp /= p;
            if (omega > 3) break;        /* early exit */
        }

        if (omega == 3 && isPrime_uint64(sum))
            push_term((seq_t)last_checked);
    }

    return seq_array[n - 1];
}
