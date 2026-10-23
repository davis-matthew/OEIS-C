/*
 * A193063  Primes p for which there is no prime q == 1 (mod 3) that is
 *          smaller than p and is a quadratic residue modulo p.
 * https://oeis.org/A193063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A193063 – primes p for which there is no prime q ≡ 1 (mod 3) smaller
    than p that is a quadratic residue modulo p.
    The sequence is finite: 2, 3, 5, 7, 11, 13.  For n > 6 the
    function returns 0. */

/*  Helper: modular exponentiation (a^e mod m) for 64‑bit arguments. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e) {
        if (e & 1)
            r = (uint64_t)((__uint128_t)r * a % mod);
        a = (uint64_t)((__uint128_t)a * a % mod);
        e >>= 1;
    }
    return r;
}

/*  Simple dynamic array helpers for uint64_t and seq_t. */
struct darr_uint64_t { uint64_t *buf; size_t sz; size_t cap; };
static void grow_uint64(struct darr_uint64_t *a)
{
    size_t new_cap = a->cap ? a->cap << 1 : 8;
    uint64_t *p = (uint64_t *)realloc(a->buf, new_cap * sizeof(uint64_t));
    if (!p) exit(1);
    a->buf = p;  a->cap = new_cap;
}

struct darr_seq_t { seq_t *buf; size_t sz; size_t cap; };
static void grow_seq(struct darr_seq_t *a)
{
    size_t new_cap = a->cap ? a->cap << 1 : 8;
    seq_t *p = (seq_t *)realloc(a->buf, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    a->buf = p;  a->cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  The requested OEIS function                                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static struct darr_uint64_t primes   = {NULL,0,0};   /* all primes seen so far */
    static struct darr_uint64_t qprimes  = {NULL,0,0};   /* primes ≡ 1 (mod 3) */
    static struct darr_seq_t   results  = {NULL,0,0};   /* sequence values */
    static uint64_t candidate = 2;                      /* next odd number to test */
    static bool finished = false;                       /* all terms found */

    /* Continue generating prime candidates until we have enough results
       or the sequence is proven finished. */
    while (!finished && results.sz < n) {
        /*  --- primality test for candidate ---   */
        bool is_prime = true;
        for (size_t i = 0; i < primes.sz; ++i) {
            uint64_t p = primes.buf[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Store the new prime. */
            if (primes.sz == primes.cap) grow_uint64(&primes);
            primes.buf[primes.sz++] = candidate;

            /* Check whether any earlier q ≡ 1 (mod 3) is a quadratic residue. */
            bool ok = true;   /* assume candidate qualifies */
            for (size_t j = 0; j < qprimes.sz && ok; ++j) {
                uint64_t q = qprimes.buf[j];
                uint64_t legendre = mod_pow64(q, (candidate - 1) / 2, candidate);
                if (legendre == 1) ok = false;   /* found a QR */
            }

            if (ok) {
                /* Record the term. */
                if (results.sz == results.cap) grow_seq(&results);
                results.buf[results.sz++] = (seq_t)candidate;

                /* The sequence stops after 13. */
                if (results.sz >= 6 && candidate > 13) finished = true;
            }

            /* Keep q primes (q % 3 == 1). */
            if (candidate % 3 == 1) {
                if (qprimes.sz == qprimes.cap) grow_uint64(&qprimes);
                qprimes.buf[qprimes.sz++] = candidate;
            }
        }

        /* Advance candidate: start with 3, then only odd numbers. */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    /* Return the requested term or 0 if it does not exist. */
    if (n <= results.sz)
        return results.buf[n - 1];
    return (seq_t)0;
}
