/*
 * A106089  Primes p such that 7*p + 8 and 8*p + 7 are primes.
 * https://oeis.org/A106089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that 7*p + 8 and 8*p + 7 are primes
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------
   64‑bit Miller–Rabin primality test
   ------------------------------------------------------------ */
static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t powmod64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod64(r, a, m);
        a = mulmod64(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* quick trial division */
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL
    };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------
   Storage of sequence terms
   ------------------------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> seq_vals;
static uint64_t gen_next = 2;          /* next candidate prime */
#else
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;             /* number of terms stored */
static size_t seq_cap = 0;             /* current array capacity */
static uint64_t gen_next = 2;          /* next candidate prime */

static void grow_seq(void)
{
    size_t new_cap = (seq_cap == 0 ? 64 : seq_cap << 1);
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* out of memory */
    seq_vals = tmp;
    seq_cap = new_cap;
}
#endif

static void add_term(seq_t p)
{
#ifdef __cplusplus
    seq_vals.push_back(p);
#else
    if (seq_len == seq_cap) grow_seq();
    seq_vals[seq_len++] = p;
#endif
}

/* ------------------------------------------------------------
   Public function
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    size_t current_size;
#ifdef __cplusplus
    current_size = seq_vals.size();
#else
    current_size = seq_len;
#endif

    while (current_size < (size_t)n) {
        if (isPrime64(gen_next)) {
            uint64_t a_val = 7ULL * gen_next + 8ULL;
            uint64_t b_val = 8ULL * gen_next + 7ULL;
            if (isPrime64(a_val) && isPrime64(b_val)) {
                add_term((seq_t)gen_next);
                ++current_size;
            }
        }
        gen_next += (gen_next == 2 ? 1 : 2);  /* skip even numbers after 2 */
    }

#ifdef __cplusplus
    return seq_vals[n - 1];
#else
    return seq_vals[n - 1];
#endif
}
