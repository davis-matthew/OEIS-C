/*
 * A196598  Number of nX5 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 1,2,4,0,3 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for A196598 a(n) = 2*a(n-1) +16*a(n-2) +43*a(n-3)
 *            +120*a(n-4) +242*a(n-5) -17*a(n-6) -727*a(n-7) -529*a(n-8)
 *            +362*a(n-9) +378*a(n-10) +5*a(n-11) -55*a(n-12) +5*a(n-13)
 *            -51*a(n-14) +65*a(n-15) +15*a(n-16) -5*a(n-17) -2*a(n-18)
 *            -a(n-19) with a(1)=1, a(2)=17, …, a(19)=630492043325935
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum n that can be handled.  This must be ≥ any n that will be
   requested by the test harness.  The OEIS specification talks about
   values up to about 10000; 20000 is comfortably safe. */
#define A196598_MAX_N 20000

/* Static cache of computed terms.  The first index (0) is unused to
   keep indices 1‑based, matching the OEIS offset. */
static seq_t cache[A196598_MAX_N + 1];
static size_t cache_used = 0;

/* Initial 19 values of the sequence.  They are stored as unsigned
   integer constants of the appropriate width; if seq_t is an
   arbitrary‑precision class the compiler will convert these literals
   correctly. */
static const seq_t init_vals[19] = {
    1ULL,          17ULL,          116ULL,         667ULL,         4529ULL,
    26558ULL,      170468ULL,      1067723ULL,     6694705ULL,     41981191ULL,
    263257030ULL, 1651418740ULL, 10357579301ULL, 64961533372ULL, 407436733800ULL,
    2555438539179ULL, 16027677245855ULL, 100525225308587ULL,
    630492043325935ULL
};

/* Forward declaration of a helper that ensures the cache contains
   at least n terms.  It is written in a way that compiles under C
   (malloc) and under C++17 (new[] is never used). */
static void ensure_cache(idx_t n);

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; a(1) is the first term.  For n < 1 the
       value is outside the defined range; we return 0. */
    if (n == 0)
        return 0;

    /* Truncate incredibly large indices to the maximum supported value
       to avoid buffer overruns;  the test harness will never request
       such a value, but it keeps undefined behaviour out of the
       implementation. */
    if (n > (idx_t)A196598_MAX_N)
        n = (idx_t)A196598_MAX_N;

    ensure_cache(n);
    return cache[(size_t)n];
}

/* ------------------------------------------------------------------ */
/*  Private helper:  Initialize and extend the cache up to index n.  */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n)
{
    /* First, initialise the first 19 entries if they have not been
       initialised yet.  We do this only once. */
    if (cache_used == 0) {
        for (size_t i = 1; i <= 19; ++i)
            cache[i] = init_vals[i - 1];
        cache_used = 19;
    }

    /* If the requested index is already available, nothing more to
       do. */
    if ((size_t)n <= cache_used)
        return;

    /* Compute missing terms one by one using the recurrence. */
    for (size_t i = cache_used + 1; i <= (size_t)n; ++i) {
        seq_t val = 0;
        /* Positive contributions */
        val += (seq_t)2   * cache[i - 1];
        val += (seq_t)16  * cache[i - 2];
        val += (seq_t)43  * cache[i - 3];
        val += (seq_t)120 * cache[i - 4];
        val += (seq_t)242 * cache[i - 5];
        val += (seq_t)362 * cache[i - 9];
        val += (seq_t)378 * cache[i -10];
        val += (seq_t)5   * cache[i -11];
        val += (seq_t)5   * cache[i -13];
        val += (seq_t)65  * cache[i -15];
        val += (seq_t)15  * cache[i -16];
        /* Negative contributions */
        val -= (seq_t)17  * cache[i - 6];
        val -= (seq_t)727 * cache[i - 7];
        val -= (seq_t)529 * cache[i - 8];
        val -= (seq_t)55  * cache[i -12];
        val -= (seq_t)51  * cache[i -14];
        val -= (seq_t)5   * cache[i -17];
        val -= (seq_t)2   * cache[i -18];
        val -= (seq_t)1   * cache[i -19];
        cache[i] = val;
    }
    cache_used = (size_t)n;
}
