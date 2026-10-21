/*
 * A082373  Table with 4 consecutive primes prime(k), prime(k+1), prime(k+2)
 *          and prime(k+3) in a row such that prime(k)^x + prime(k+1)^x ==
 *          prime(k+2) (mod prime(k+3)) has no solution x.
 * https://oeis.org/A082373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn, tabf
 * Formula:   A082373: For each consecutive quadruple of primes (p_k, p_{k+1},
 *            p_{k+2}, p_{k+3}) the quadruple is appended to the sequence iff
 *            for all integers x ≥ 1 p_k^x + p_{k+1}^x ≢ p_{k+2} (mod
 *            p_{k+3}). The sequence lists the prime values in the order they
 *            appear
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 64‑bit modular multiplication using an unsigned 128‑bit intermediate. */
static inline uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Test whether the quadruple has no solution x. */
static bool no_solution(uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4)
{
    uint64_t a = p1 % p4;
    uint64_t b = p2 % p4;
    for (uint64_t x = 1; x < p4; ++x)
    {
        if ((a + b) % p4 == p3)
            return false;                 /* solution exists */
        a = mulmod_u64(a, p1, p4);
        b = mulmod_u64(b, p2, p4);
    }
    return true;                          /* no solution */
}

/* -------- Prime sieve (naïve trial division) -------- */
static uint64_t *prime_list = NULL;   /* dynamic array of primes */
static size_t prime_cnt = 0;          /* number of primes stored */
static size_t prime_cap = 0;          /* capacity of prime_list */
static uint64_t last_checked = 1;     /* last candidate tested */

/* Ensure that prime_list contains at least 'needed' primes. */
static void build_primes_up_to(size_t needed)
{
    while (prime_cnt < needed)
    {
        ++last_checked;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= last_checked; ++i)
        {
            if (last_checked % prime_list[i] == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = last_checked;
        }
    }
}

/* -------- Sequence storage -------- */
static seq_t *seq_buf = NULL;   /* dynamic array of output terms (seq_t) */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* capacity of seq_buf */

/* Append a prime value to the sequence. */
static void append_seq(uint64_t val)
{
    if (seq_len == seq_cap)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_buf = tmp;
        seq_cap = new_cap;
    }
    seq_buf[seq_len++] = (seq_t)val;
}

/* -------- Main sequence function -------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined; safeguard */

    /* index of the first prime in the current window, starting at prime 3 */
    static idx_t scan_idx = 1;

    /* Grow sequence until we have at least n terms. */
    while (seq_len < (size_t)n)
    {
        /* Ensure we have at least scan_idx + 4 primes (indices 0..scan_idx+3). */
        build_primes_up_to((size_t)scan_idx + 4);

        uint64_t p1 = prime_list[(size_t)scan_idx];
        uint64_t p2 = prime_list[(size_t)scan_idx + 1];
        uint64_t p3 = prime_list[(size_t)scan_idx + 2];
        uint64_t p4 = prime_list[(size_t)scan_idx + 3];

        if (no_solution(p1, p2, p3, p4))
        {
            append_seq(p1);
            append_seq(p2);
            append_seq(p3);
            append_seq(p4);
        }
        ++scan_idx;               /* next overlapping window */
    }

    return seq_buf[(size_t)n - 1];
}
