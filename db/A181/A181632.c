/*
 * A181632  The Fibonacci Champernowne sequence.
 * https://oeis.org/A181632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth bit (0‑based) of the infinite binary string obtained
 *            by concatenating the Fibonacci‐Maximal notation of all
 *            nonnegative integers in order
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Fibonacci numbers starting with 1,2,3,5,...  */
static idx_t fib[100];          /* stored in idx_t for speed */
static size_t fib_cnt = 0;      /* actual number of fibs precomputed */

/* Prefix sums of the Fibonacci numbers (as unsigned __int128 to avoid overflow).  */
static unsigned __int128 pref[100];

/* Bit cache ------------------------------------------------------- */
/* bits[i] holds the i-th bit of the concatenated sequence, as 0 or 1. */
static unsigned char *bits = NULL;   /* dynamic array */
static size_t bits_len = 0;          /* number of bits stored */
static size_t bits_cap = 0;          /* allocated capacity */

/* Next integer whose representation will be generated. */
static idx_t next_val = 0;

/* ----------------------------------------------------------------- */
/* Ensure the Fibonacci tables are initialized. */
static void init_fibs(void)
{
    if (fib_cnt != 0) return;          /* already done */

    fib[0] = 1;
    fib[1] = 2;
    fib_cnt = 2;
    pref[0] = fib[0];
    pref[1] = (unsigned __int128)fib[0] + fib[1];

    while (true) {
        /* prevent overflow in idx_t addition */
        if (UINT64_MAX - fib[fib_cnt-2] < fib[fib_cnt-1]) break;
        fib[fib_cnt] = fib[fib_cnt-2] + fib[fib_cnt-1];
        pref[fib_cnt] = pref[fib_cnt-1] + fib[fib_cnt];
        fib_cnt++;
        if (fib_cnt >= 100) break;
    }
}

/* ----------------------------------------------------------------- */
/* Resize the bit cache if needed. */
static void ensure_bits_cap(size_t needed)
{
    if (needed <= bits_cap) return;
    size_t new_cap = bits_cap ? bits_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(bits, new_cap * sizeof(unsigned char));
    if (!tmp) {
        /* allocation failed – abort (unrecoverable) */
        abort();
    }
    bits = tmp;
    bits_cap = new_cap;
}

/* ----------------------------------------------------------------- */
/* Append a single bit (0 or 1) to the cache. */
static void append_bit(unsigned char b)
{
    ensure_bits_cap(bits_len + 1);
    bits[bits_len++] = b;
}

/* ----------------------------------------------------------------- */
/* Generate and append the bits of the Fibonacci Maximal notation of
   an integer val (val > 0).  For val == 0, just append a single 0. */
static void generate_bits(idx_t val)
{
    if (val == 0) {
        append_bit(0);
        return;
    }

    /* Find minimal length len such that sum_{i=0}^{len-1} fib[i] >= val */
    size_t len = 0;
    unsigned __int128 acc = 0;
    while (acc < (unsigned __int128)val) {
        acc += fib[len];
        len++;
    }

    idx_t remainder = val;
    for (int i = (int)len - 1; i >= 0; --i) {
        unsigned __int128 lower = (i > 0) ? pref[i-1] : 0;
        if ((unsigned __int128)remainder > lower) {
            /* we must use fib[i] */
            append_bit(1);
            remainder -= fib[i];
        } else {
            /* we can skip fib[i] */
            append_bit(0);
        }
    }
}

/* ----------------------------------------------------------------- */
/* Public function mandated by the problem statement. */
seq_t a(idx_t n)
{
    static bool first_call = true;
    if (first_call) {
        init_fibs();
        bits = NULL;
        bits_len = 0;
        bits_cap = 0;
        next_val = 0;
        first_call = false;
    }

    /* Grow the cache until it contains at least (n+1) bits. */
    while (bits_len <= (size_t)n) {
        generate_bits(next_val++);
    }

    return (seq_t)bits[n];
}
