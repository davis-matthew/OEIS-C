/*
 * A096287  Number of iterations of n -> n + (product of nonzero digits of n)
 *          needed for the trajectory of n to join the trajectory of A063108.
 * https://oeis.org/A096287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096287 – Number of iterations of n → n + (product of non‑zero digits of n)
   needed for the trajectory of n to join the trajectory of 1 (A063108). */

/*  The function also builds the orbit of 1 lazily.  The orbit is strictly
    increasing, so a number belongs to it iff it is contained in the
    pre‑computed array.  The algorithm runs in O(k log m) where k is the
    number of steps from n to the orbit and m is the size of the orbit
    seen so far.  For all n ≤ 10⁴ the program finishes in far less than 2 s.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Implementation depends on the types defined by the build system.  */
extern int main();  /* The build system supplies seq_t and idx_t */

/* ---- Helper: product of non‑zero decimal digits -------------------------------- */
static seq_t digit_prod_nonzero(seq_t x)
{
    seq_t prod = (seq_t)1;
    while (x) {
        seq_t d = x % (seq_t)10;
        if (d) prod *= d;
        x /= (seq_t)10;
    }
    return prod;
}

/* ---- Lazy construction of the orbit of 1 ----------------------------------------- */
static seq_t *orbit      = NULL;   /* sorted increasing values */
static size_t orbit_size = 0;      /* number of values stored */
static size_t orbit_cap  = 0;      /* allocated capacity */

/* Append a new term to the orbit array */
static void append_orbit(seq_t val)
{
    if (orbit_size >= orbit_cap) {
        size_t new_cap = orbit_cap ? orbit_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(orbit, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* unrecoverable allocation failure */
        orbit = tmp;
        orbit_cap = new_cap;
    }
    orbit[orbit_size++] = val;
}

/* Ensure orbit contains all terms up to at least 'limit' (inclusive).  */
static void ensure_orbit_upto(seq_t limit)
{
    if (orbit_size == 0) {
        /* initialise with the first term 1 */
        append_orbit((seq_t)1);
    }
    while (orbit[orbit_size - 1] < limit) {
        seq_t cur = orbit[orbit_size - 1];
        seq_t next = cur + digit_prod_nonzero(cur);
        append_orbit(next);
    }
}

/* Binary search in the orbit array; returns true if 'x' exists.  */
static bool in_orbit(seq_t x)
{
    size_t low = 0, high = orbit_size;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (orbit[mid] < x)
            low = mid + 1;
        else
            high = mid;
    }
    return (low < orbit_size && orbit[low] == x);
}

/* ---- The required function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t steps = (seq_t)0;

    /* Make sure we already know the orbit up to the current value */
    ensure_orbit_upto(cur);

    while (!in_orbit(cur)) {
        cur = cur + digit_prod_nonzero(cur);
        steps++;
        ensure_orbit_upto(cur);   /* extend orbit if needed */
    }
    return steps;
}
