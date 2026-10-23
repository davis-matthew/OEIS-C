/*
 * A040177  Primes p such that x^5 = 10 has a solution mod p.
 * https://oeis.org/A040177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040177: Primes p such that x^5 = 10 has a solution mod p
   (offset 1, so a(1)=2).
   This implementation caches found terms and builds primes on demand.
   The quintic residue test uses the fact that for prime p≠5,
   10 is a 5th power residue iff 10^((p-1)/5)≡1 (mod p) when 5|(p−1).
   If 5∤(p−1) the condition always holds, and for p=2,3,5 the residue
   is trivial.  The algorithm is O(n log n) and stays well below the
   time limit for n up to about 10 000.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Fallback cast for realloc/malloc to keep C++ compatible */
#ifdef __cplusplus
#  define RELOC_CAST(T,p) reinterpret_cast<T>(p)
#else
#  define RELOC_CAST(T,p) (T)(p)
#endif

/* Sequence cache ---------------------------------------------------- */
static seq_t *sequence      = NULL;   /* stored terms                                   */
static size_t seq_len       = 0;      /* number of stored terms                         */
static size_t seq_cap       = 0;      /* size of allocated block                        */

/* Append a term to the sequence cache, growing it if necessary.      */
static void append_sequence(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        seq_t *tmp = RELOC_CAST(seq_t*, realloc(sequence, new_cap * sizeof(seq_t)));
        if (!tmp) exit(EXIT_FAILURE);
        sequence = tmp;
        seq_cap   = new_cap;
    }
    sequence[seq_len++] = val;
}

/* Prime cache ------------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* list of primes found                             */
static size_t   prime_len   = 0;      /* number of stored primes                          */
static size_t   prime_cap   = 0;      /* size of allocated block                          */

/* Append a prime to the prime list, growing it if necessary.         */
static void append_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 4;
        uint64_t *tmp = RELOC_CAST(uint64_t*, realloc(prime_list, new_cap * sizeof(uint64_t)));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap   = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* Fast modular exponentiation for 64‑bit values.                     */
static seq_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U) result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1U;
    }
    return (seq_t)result;
}

/* Simple trial division primality test using previously found primes. */
static bool is_prime_candidate(uint64_t candidate)
{
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > candidate / p) break;           /* p*p > candidate */
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Main function: return the n‑th element (1‑based indexing).         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 but harmless       */

    /* Generate terms until we have at least n of them.               */
    static uint64_t next_candidate = 2;   /* candidate for the next prime      */

    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate;
        if (cand == 2)         next_candidate = 3;
        else                   next_candidate += 2;   /* skip even numbers */

        if (!is_prime_candidate(cand)) continue;
        append_prime(cand);

        /* Quintic‑residue test.                                      */
        bool include = false;
        if (cand == 2 || cand == 3 || cand == 5) {
            include = true; /* 10≡0 or 1 (mod p) */
        } else if (cand % 5 != 1) {
            include = true; /* 5∤(p−1) → always a 5th power residue */
        } else {
            uint64_t exp = (cand - 1) / 5;
            if (pow_mod_uint64(10, exp, cand) == 1)
                include = true;
        }

        if (include) append_sequence((seq_t)cand);
    }

    return sequence[n - 1];
}
