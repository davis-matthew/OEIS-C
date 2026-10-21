/*
 * A099647  Function f[n]=1+Sum[digit^2 of n] is iterated as in A099646.
 *          Values x for which A099646[x]=1 are listed here. These terms are
 *          analogous to happy-numbers [=A007770].
 * https://oeis.org/A099647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A099647 – numbers that eventually reach a fixed point
 * of f(x) = 1 + Σ (digit^2).  The function returns the n‑th term
 * (1‑based indexing).  It works for any seq_t size, including 8‑, 16‑,
 * 32‑, 64‑, 128‑bit or arbitrary‑precision types, and for any idx_t
 * (typically uint64_t).  The sequence is built by checking each natural
 * number for the fixed‑point property, and the results are cached in
 * a dynamic array so that calls may arrive in any order. */

/* Necessary standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------
 * Helper: sum of squares of decimal digits of a seq_t number
 * ------------------------------------------------------------ */
static seq_t digit_square_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        seq_t d = x % 10;      /* decimal digit */
        sum += d * d;
        x /= 10;
    }
    return sum;
}

/* ------------------------------------------------------------
 * Helper: test whether a start value eventually reaches a fixed point.
 * Uses only local arrays; no global status caching.  Works for
 * arbitrary seq_t types as long as >, %, /, +, * are defined.
 * ------------------------------------------------------------ */
static bool reaches_fixed_point(seq_t start)
{
    /* Use a static buffer large enough for the longest expected
     * cycle/ transient.  4096 is comfortably above the known limits. */
    static const size_t MAX_PATH = 4096;
    seq_t path[MAX_PATH];
    size_t depth = 0;

    /* Initialise the path with the starting value. */
    path[depth++] = start;
    seq_t current = start;

    while (1) {
        seq_t next = 1 + digit_square_sum(current);

        /* Fixed point found. */
        if (next == current)
            return true;

        /* Look for repetition; if found we are in a non‑fixed
         * cycle and the number is bad. */
        for (size_t i = 0; i < depth; ++i) {
            if (path[i] == next)
                return false;
        }

        /* Safety guard: if the path becomes too long we stop.
         * It should never happen for the known range. */
        if (depth >= MAX_PATH)
            return false;

        /* Record the new value and continue. */
        path[depth++] = next;
        current = next;
    }
}

/* ------------------------------------------------------------
 * Sequence storage: dynamic array of seq_t
 * ------------------------------------------------------------ */
static seq_t *seqbuf      = NULL;  /* array of found terms */
static size_t seq_cap     = 0;     /* current array capacity */
static size_t seq_len     = 0;     /* number of terms stored */

/* Append a new term to the sequence buffer, growing it if necessary. */
static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(seqbuf, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* memory allocation failed */
        seqbuf = tmp;
        seq_cap = new_cap;
    }
    seqbuf[seq_len++] = val;
}

/* ------------------------------------------------------------
 * Main entry point
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is defined for n ≥ 1 only */
        return 0;

    /* Next natural number to test */
    static idx_t next_start = 1;

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        if (reaches_fixed_point((seq_t)next_start))
            append_seq((seq_t)next_start);
        ++next_start;
    }

    return seqbuf[n - 1];
}
