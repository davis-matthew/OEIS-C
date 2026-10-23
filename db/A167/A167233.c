/*
 * A167233  Prime anagrams of 12345769.
 * https://oeis.org/A167233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   This function enumerates all primes that are permutations of the
 *            digits 1,2,3,4,5,6,7,9 and returns the n‑th in ascending order,
 *            matching OEIS A167233
 * Range:     u8 none, u16 none, u32 n <= 4333, u64 n <= 4333, u128 n <= 4333,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- primality test for 32‑bit unsigned integers ---------- */
static bool is_prime32(uint32_t n)
{
    if (n < 2) return false;

    /* trial division by small primes */
    static const uint32_t primes[] = {
        2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u, 31u,
        37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u, 73u,
        79u, 83u, 89u, 97u
    };
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        uint32_t p = primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Miller‑Rabin for n < 2^32: bases 2, 7, 61 are sufficient */
    uint32_t d = n - 1;
    int s = 0;
    while ((d & 1u) == 0u) {
        d >>= 1u;
        ++s;
    }

    static const uint32_t bases[] = {2u, 7u, 61u};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint32_t a = bases[i] % n;
        if (a == 0u) continue;

        /* modpow(a, d, n) */
        uint64_t x = 1, base = a;
        uint32_t exp = d;
        while (exp > 0) {
            if (exp & 1u) x = ((unsigned __int128)x * base) % n;
            base = ((unsigned __int128)base * base) % n;
            exp >>= 1u;
        }

        if (x == 1u || x == n - 1u) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = ((unsigned __int128)x * x) % n;
            if (x == n - 1u) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- next lexicographic permutation (in place) ---------- */
static bool next_permutation(char *arr, int n)
{
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) --i;
    if (i < 0) return false;

    int j = n - 1;
    while (arr[j] <= arr[i]) --j;

    char tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;

    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
        ++l;
        --r;
    }
    return true;
}

/* ---------- caching of the prime anagrams ---------- */
static uint32_t *prime_cache = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static bool   initialized = false;

/* comparison function for qsort */
static int cmp_uint32(const void *a, const void *b)
{
    uint32_t va = *(const uint32_t *)a;
    uint32_t vb = *(const uint32_t *)b;
    return (int)(va - vb);
}

/* generate all primes that are permutations of 1,2,3,4,5,6,7,9 */
static void generate_primes(void)
{
    char digits[8] = {'1','2','3','4','5','6','7','9'};

    do {
        uint32_t val = 0;
        for (int i = 0; i < 8; ++i)
            val = val * 10u + (uint32_t)(digits[i] - '0');

        if (is_prime32(val)) {
            if (prime_count == prime_cap) {
                prime_cap = (prime_cap == 0) ? 128 : prime_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_cache, prime_cap * sizeof(uint32_t));
                if (!tmp) {
                    /* memory allocation failed – abort silently */
                    return;
                }
                prime_cache = tmp;
            }
            prime_cache[prime_count++] = val;
        }
    } while (next_permutation(digits, 8));

    /* sort in ascending numeric order */
    qsort(prime_cache, prime_count, sizeof(uint32_t), cmp_uint32);
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        generate_primes();
    }

    if (n == 0 || (size_t)n > prime_count)
        return 0;         /* out of range – arbitrary choice of 0 */

    return (seq_t)prime_cache[(size_t)(n - 1)];
}
