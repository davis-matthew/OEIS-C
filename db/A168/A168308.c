/*
 * A168308  The fifth left hand column of triangle A167591.
 * https://oeis.org/A168308
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - 180*a(n-2) + 960*a(n-3) - 3360*a(n-4) * +
 *            8064*a(n-5) - 13440*a(n-6) + 15360*a(n-7) - 11520*a(n-8) * +
 *            5120*a(n-9) - 1024*a(n-10), with * a(5)=525, a(6)=12396,
 *            a(7)=162740, a(8)=1537216, * a(9)=11589216, a(10)=73898880,
 *            a(11)=413745024, * a(12)=2087500800, a(13)=9672309504, a(1...
 * Range:     u8 none, u16 n <= 6, u32 n <= 12, u64 n <= 31, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10050];          /* pre‑allocate enough for n up to ~10000 */
static idx_t last_computed = 0;     /* index of the last term stored in cache (0‑based) */

/* initial 10 terms of the sequence, cast to seq_t to work for arbitrary‑precision types */
static const seq_t init_vals[10] = {
    (seq_t)525ULL,
    (seq_t)12396ULL,
    (seq_t)162740ULL,
    (seq_t)1537216ULL,
    (seq_t)11589216ULL,
    (seq_t)73898880ULL,
    (seq_t)413745024ULL,
    (seq_t)2087500800ULL,
    (seq_t)9672309504ULL,
    (seq_t)41745859584ULL
};

seq_t a(idx_t n)
{
    if (n < 5) return 0;                /* undefined for n<5 */

    idx_t off = n - 5;                  /* array offset for term n */

    if (off < 10)                       /* directly return initial terms */
        return init_vals[off];

    /* ensure the initial 10 terms are stored in cache */
    if (last_computed < 9) {
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
        last_computed = 9;
    }

    /* compute terms up to the requested index using the 10‑term recurrence */
    while (last_computed < off) {
        seq_t val =
           (seq_t)20 * cache[last_computed]
         - (seq_t)180 * cache[last_computed - 1]
         + (seq_t)960 * cache[last_computed - 2]
         - (seq_t)3360 * cache[last_computed - 3]
         + (seq_t)8064 * cache[last_computed - 4]
         - (seq_t)13440 * cache[last_computed - 5]
         + (seq_t)15360 * cache[last_computed - 6]
         - (seq_t)11520 * cache[last_computed - 7]
         + (seq_t)5120  * cache[last_computed - 8]
         - (seq_t)1024  * cache[last_computed - 9];

        ++last_computed;
        cache[last_computed] = val;
    }

    return cache[off];
}
