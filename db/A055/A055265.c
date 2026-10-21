/*
 * A055265  a(n) is the smallest positive integer not already in the sequence
 *          such that a(n)+a(n-1) is prime, starting with a(1)=1.
 * https://oeis.org/A055265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055265: a(1) = 1.
   For n > 1, a(n) is the smallest positive integer not already in the
   sequence such that a(n)+a(n-1) is prime.  The implementation uses a
   small prime test (deterministic Miller–Rabin for 64‑bit values) which
   works for arbitrary‑precision seq_t as well.  The sequence is built
   incrementally and cached for fast repeated access. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <string.h>   /* memset */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */

/* ------ helper: modular exponentiation ------------------------------------ */
static seq_t modpow(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & (seq_t)1)
            result = (result * base) % mod;
        exp >>= 1;
        base   = (base * base) % mod;
    }
    return result;
}

/* ------ helper: deterministic Miller–Rabin for arbitrary precision ----- */
static bool is_prime_seq(seq_t n)
{
    /* small cases */
    if (n < (seq_t)2) return false;
    if (n % (seq_t)2 == 0) return n == (seq_t)2;

    /* quick check against small primes */
    static const uint64_t small_primes[] = {3,5,7,11,13,17,19,23,29,31,37};
    size_t sp_cnt = sizeof(small_primes)/sizeof(small_primes[0]);
    for (size_t i = 0; i < sp_cnt; ++i) {
        uint64_t p = small_primes[i];
        seq_t pp = (seq_t)p;
        if (n == pp) return true;
        if (n % pp == 0) return false;
    }

    /* Miller–Rabin */
    seq_t d = n - (seq_t)1;
    unsigned s = 0;
    while ((d & (seq_t)1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases valid for 64‑bit integers; using the same set for larger
       numbers gives a very high confidence of primality. */
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    size_t b_cnt = sizeof(bases)/sizeof(bases[0]);

    for (size_t i = 0; i < b_cnt; ++i) {
        uint64_t a_val = bases[i];
        if ((seq_t)a_val >= n) continue;   /* skip bases >= n */
        seq_t a = (seq_t)a_val;
        seq_t x = modpow(a, d, n);
        if (x == (seq_t)1 || x == n - (seq_t)1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = modpow(x, 2, n);
            if (x == n - (seq_t)1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------ cache for the sequence ------------------------------------------- */
static seq_t *seq        = NULL;
static size_t seq_len    = 0;
static size_t seq_alloc  = 0;

static unsigned char *used = NULL;     /* 0 = unused, 1 = used */
static size_t used_alloc  = 0;
static size_t min_unused  = 1;         /* smallest integer not yet used */

/* grow the sequence array if necessary */
static void ensure_seq(size_t needed)
{
    if (needed <= seq_alloc) return;
    size_t new_alloc = seq_alloc ? seq_alloc * 2 : 256;
    while (new_alloc < needed) new_alloc <<= 1;
    seq = (seq_t *)realloc(seq, new_alloc * sizeof(seq_t));
    seq_alloc = new_alloc;
}

/* grow the used bitmap if necessary */
static void ensure_used(size_t needed)
{
    if (needed <= used_alloc) return;
    size_t new_alloc = used_alloc ? used_alloc * 2 : 256;
    while (new_alloc < needed) new_alloc <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_alloc);
    if (tmp) {
        memset(tmp + used_alloc, 0, new_alloc - used_alloc);
        used = tmp;
        used_alloc = new_alloc;
    }
}

/* -------------------------------------------------------------------------- */
/* Main sequence function                                                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

/* initialise with a(1)=1 */
    if (seq_len == 0) {
        ensure_seq(1);
        seq[0] = (seq_t)1;
        seq_len = 1;

        ensure_used(2);          /* allow index 1 */
        used[1] = 1;             /* mark 1 as used */
        min_unused = 2;
    }

/* extend sequence up to requested index */
    size_t up_to = (size_t)n;
    if (seq_len < up_to) {
        for (size_t i = seq_len; i < up_to; ++i) {
            seq_t prev = seq[i - 1];
            size_t candidate = min_unused;

            /* search for next unused integer whose sum with prev is prime */
            while (true) {
                ensure_used(candidate + 1);
                if (!used[candidate]) {
                    seq_t sum = prev + (seq_t)candidate;
                    if (is_prime_seq(sum))
                        break;
                }
                ++candidate;
            }

            /* store chosen term */
            ensure_seq(i + 1);
            seq[i] = (seq_t)candidate;

            /* mark as used */
            ensure_used(candidate + 1);
            used[candidate] = 1;

            /* advance min_unused if it was taken */
            if (candidate == min_unused) {
                do {
                    ++min_unused;
                } while (min_unused < used_alloc && used[min_unused]);
            }
        }
        seq_len = up_to;
    }

    return seq[n - 1];
}
