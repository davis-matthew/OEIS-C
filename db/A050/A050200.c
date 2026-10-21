/*
 * A050200  Let p = prime(n). Then a(n) = p + (next prime >= p+1) + (next
 *          prime >= p+3).
 * https://oeis.org/A050200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + nextPrime(prime(n)+1) + nextPrime(prime(n)+3)
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is 0‑based (prime(0)=2).  The function computes primes on demand
   using deterministic Miller–Rabin for 64‑bit integers, storing the
   primes found so far in a dynamic array.  For each request it extends
   the stored list as necessary.  This satisfies the O(n) time requirement
   for n up to about 10⁴ with far less than 2 s per call. */

#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------*/
/* Miller–Rabin primality test for 64‑bit numbers             */
/*------------------------------------------------------------*/
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic bases for < 2^64 */
static const uint64_t miller_bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL};

static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    /* small primes first */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return 1;
        if (n % p == 0) return 0;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < sizeof(miller_bases)/sizeof(miller_bases[0]); ++i) {
        uint64_t a = miller_bases[i];
        if (a >= n) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int comp = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

/*------------------------------------------------------------*/
/* Dynamic prime table                                        */
/*------------------------------------------------------------*/
static uint64_t *prime_table = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

static void extend_prime_table(void)
{
    /* Ensure space for at least one more prime */
    if (prime_capacity == 0) {
        prime_capacity = 1024;
        prime_table = (uint64_t*)malloc(prime_capacity * sizeof(uint64_t));
    } else if (prime_count >= prime_capacity) {
        prime_capacity = prime_capacity * 2;
        prime_table = (uint64_t*)realloc(prime_table, prime_capacity * sizeof(uint64_t));
    }
}

/* Generate primes until we have at least idx+1 primes (0‑based index) */
static void ensure_primes_up_to_idx(idx_t idx)
{
    while (prime_count <= (size_t)idx) {
        extend_prime_table();
        uint64_t candidate;
        if (prime_count == 0) {
            candidate = 2;
        } else {
            candidate = prime_table[prime_count - 1] + 1;
            if ((candidate & 1) == 0) candidate++;   /* skip even numbers */
        }
        while (!is_prime_uint64(candidate)) {
            candidate += 2;   /* skip even numbers */
        }
        prime_table[prime_count++] = candidate;
    }
}

/* Return prime(n) with 0‑based indexing */
static uint64_t get_prime(idx_t n)
{
    ensure_primes_up_to_idx(n);
    return prime_table[(size_t)n];
}

/* Return the first prime >= start */
static uint64_t next_prime_ge(uint64_t start)
{
    if (start <= 2) return 2;
    uint64_t candidate = start;
    if ((candidate & 1) == 0) candidate++;   /* make odd */
    while (!is_prime_uint64(candidate)) {
        candidate += 2;
    }
    return candidate;
}

/*------------------------------------------------------------*/
/* Main sequence function                                      */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 1. get p = prime(n) */
    uint64_t p = get_prime(n);
    /* 2. next prime >= p+1  -> q */
    uint64_t q = next_prime_ge(p + 1);
    /* 3. next prime >= p+3  -> r */
    uint64_t r = next_prime_ge(p + 3);
    /* 4. sum, cast to seq_t */
    return (seq_t)p + (seq_t)q + (seq_t)r;
}
