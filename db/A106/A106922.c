/*
 * A106922  Primes of the form x^2-xy+15y^2, with x and y nonnegative.
 * https://oeis.org/A106922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1074, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p that can be expressed as x^2 - x*y + 15*y^2 with non‑negative integers x,y. */
/* The function searches primes in increasing order until it has found the n‑th such prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of an unsigned __int128 value. */
static unsigned __int128 isqrt128(unsigned __int128 n)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64);   /* > sqrt(max 2^128-1) */

    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;

        if (sq > n) {
            high = mid;
        } else {
            unsigned __int128 sq_next = (mid + 1) * (mid + 1);
            if (sq_next > n) return mid;
            low = mid + 1;
        }
    }
    return low;
}

/* Test whether a prime p can be expressed in the required quadratic form. */
static bool representable(uint64_t p)
{
    if (p < 17)                /* 17 is the first such prime */
        return false;

    unsigned __int128 fourp = (unsigned __int128)4 * p;

    for (uint64_t y = 1; ; ++y) {
        unsigned __int128 yy59 = (unsigned __int128)59 * y * y;
        if (yy59 > fourp)          /* discriminant would become negative */
            break;

        unsigned __int128 disc = fourp - yy59;          /* 4p - 59*y^2 */

        unsigned __int128 sd = isqrt128(disc);
        if (sd * sd != disc)                  /* perfect square? */
            continue;

        /* (sd + y) must be even to make x integral. */
        if (((sd ^ (unsigned __int128)y) & 1) != 0)
            continue;

        return true;                         /* representation found */
    }
    return false;
}

/* Dynamic array of all primes found so far. */
static uint64_t *primes = NULL;
static size_t primes_cap = 0, primes_cnt = 0;

/* Push a new prime into the primes array. */
static void push_prime(uint64_t p)
{
    if (primes_cnt >= primes_cap) {
        size_t new_cap = (primes_cap == 0) ? 128 : primes_cap << 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
}

/* Dynamic array of representable primes (the sequence terms). */
static uint64_t *rep = NULL;
static size_t rep_cap = 0, rep_cnt = 0;

/* Push a new sequence term into rep. */
static void push_rep(uint64_t p)
{
    if (rep_cnt >= rep_cap) {
        size_t new_cap = (rep_cap == 0) ? 128 : rep_cap << 1;
        uint64_t *tmp = (uint64_t *)realloc(rep, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        rep = tmp;
        rep_cap = new_cap;
    }
    rep[rep_cnt++] = p;
}

/* Main OEIS function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n == 0 */

    static uint64_t last_checked = 1;   /* start checking from 2 */

    while (rep_cnt < (size_t)n) {
        ++last_checked;
        uint64_t m = last_checked;

        /* Trial‑division test for primality using the primes seen so far. */
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if ((unsigned __int128)p * p > m)
                break;
            if (m % p == 0) {
                isprime = false;
                break;
            }
        }

        if (isprime) {
            push_prime(m);                /* store prime for future tests */
            if (representable(m))
                push_rep(m);               /* sequence term found */
        }
    }

    return (seq_t)rep[n - 1];
}
