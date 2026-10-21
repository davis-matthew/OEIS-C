/*
 * A223956  Number of nX3 0..3 arrays with antidiagonals unimodal and rows and
 *          diagonals nondecreasing.
 * https://oeis.org/A223956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 20*a(n-1) -105*a(n-2) +51*a(n-3) +589*a(n-4)
 *            +1340*a(n-5) * -8833*a(n-6) -2855*a(n-7) +30423*a(n-8)
 *            +57774*a(n-9) * -140760*a(n-10) -176184*a(n-11) +106272*a(n-12)
 *            +544320*a(n-13) * for n > 13, with base values * a(1)=20,
 *            a(2)=295, a(3)=3471, a(4)=41006, a(5)=485714, * a(6)=5777663,
 *            a(7)...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* for uint64_t */

#ifdef __cplusplus
#include <vector>
#endif

/* Types provided by the build system. */
typedef uint64_t idx_t;   /* index type */

seq_t a(idx_t n)
{
    /* Base values for indices 1..13.  Index 0 is unused. */
    static const seq_t base[14] = {
        0ULL,              /* a(0) unused                                  */
        20ULL,             /* a(1)  */
        295ULL,            /* a(2)  */
        3471ULL,           /* a(3)  */
        41006ULL,          /* a(4)  */
        485714ULL,         /* a(5)  */
        5777663ULL,        /* a(6)  */
        68892600ULL,       /* a(7)  */
        822141033ULL,      /* a(8)  */
        9813968785ULL,     /* a(9)  */
        117159879996ULL,   /* a(10) */
        1398692077165ULL,  /* a(11) */
        16698106010927ULL, /* a(12) */
        199348287106117ULL /* a(13) */
    };

    if (n <= 13)
        return base[n];

    /* For larger n compute iteratively using the recurrence. */
#ifdef __cplusplus
    /* In C++ we can use std::vector so that constructors are invoked. */
    std::vector<seq_t> v(n + 1);
    for (idx_t i = 0; i < 14; ++i) v[i] = base[i];
    for (idx_t i = 14; i <= n; ++i)
    {
        seq_t val = (seq_t)20   * v[i-1]
                  + (seq_t)51   * v[i-3]
                  + (seq_t)589  * v[i-4]
                  + (seq_t)1340 * v[i-5]
                  + (seq_t)30423* v[i-8]
                  + (seq_t)57774* v[i-9]
                  + (seq_t)106272* v[i-12]
                  + (seq_t)544320* v[i-13]
                  - (seq_t)105  * v[i-2]
                  - (seq_t)8833 * v[i-6]
                  - (seq_t)2855 * v[i-7]
                  - (seq_t)140760* v[i-10]
                  - (seq_t)176184* v[i-11];
        v[i] = val;
    }
    return v[n];
#else
    /* In C we allocate raw memory.  The seq_t type is assumed to be
       trivially constructible (e.g. an unsigned integer type). */
    size_t sz = (size_t)(n + 1);
    seq_t *buf = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!buf) return (seq_t)0;        /* allocation failure: return 0 */
    for (idx_t i = 0; i < 14; ++i) buf[i] = base[i];
    for (idx_t i = 14; i <= n; ++i)
    {
        seq_t val = (seq_t)20   * buf[i-1]
                  + (seq_t)51   * buf[i-3]
                  + (seq_t)589  * buf[i-4]
                  + (seq_t)1340 * buf[i-5]
                  + (seq_t)30423* buf[i-8]
                  + (seq_t)57774* buf[i-9]
                  + (seq_t)106272* buf[i-12]
                  + (seq_t)544320* buf[i-13]
                  - (seq_t)105  * buf[i-2]
                  - (seq_t)8833 * buf[i-6]
                  - (seq_t)2855 * buf[i-7]
                  - (seq_t)140760* buf[i-10]
                  - (seq_t)176184* buf[i-11];
        buf[i] = val;
    }
    seq_t result = buf[n];
    free(buf);
    return result;
#endif
}
