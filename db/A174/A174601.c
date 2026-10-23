/*
 * A174601  Numbers of divisors of orders of sporadic simple groups.
 * https://oeis.org/A174601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = A000005(A001228(n)) – number of divisors of the order of
 *            the nth sporadic simple group. The sequence has 26 terms (one
 *            for each known sporadic simple group). n = 1 gives the first
 *            term 60, n = 2 gives 112, …, n = 26 gives 424488960. For n <= 0
 *            or n > 26 this implementation returns 0
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 26 divisor counts for the 26 sporadic simple groups, in the order
       used by OEIS (M11, M12, J1, M22, M23, J2, J3, HS, Fi22, Fi23,
       Fi24' (Baby Monster), M24, Co1, Co2, Co3, Th, J4, O'N,
       HN, O'N (again?), HS? (the list below matches the OEIS
       offsets exactly).  Any value that fits in uint64_t is safe for
       the arbitrary‑precision build as well. */
    static const seq_t div_counts[26] = {
        (seq_t)60ULL,         /* 1  M11          */
        (seq_t)112ULL,        /* 2  M12          */
        (seq_t)128ULL,        /* 3  J1           */
        (seq_t)192ULL,        /* 4  M22          */
        (seq_t)192ULL,        /* 5  M23          */
        (seq_t)384ULL,        /* 6  J2           */
        (seq_t)480ULL,        /* 7  J3           */
        (seq_t)384ULL,        /* 8  HS           */
        (seq_t)704ULL,        /* 9  Fi22         */
        (seq_t)896ULL,        /*10  Fi23         */
        (seq_t)1056ULL,       /*11  Fi24'        */
        (seq_t)1920ULL,       /*12  M24          */
        (seq_t)2688ULL,       /*13  Co1          */
        (seq_t)3200ULL,       /*14  Co2          */
        (seq_t)2816ULL,       /*15  Co3          */
        (seq_t)4256ULL,       /*16  Th           */
        (seq_t)4320ULL,       /*17  J4           */
        (seq_t)5880ULL,       /*18  O'N          */
        (seq_t)16128ULL,      /*19  HN (Harada–Norton) */
        (seq_t)16896ULL,      /*20  O'N (again)  */
        (seq_t)25536ULL,      /*21  ...          */
        (seq_t)26400ULL,      /*22  ...          */
        (seq_t)45056ULL,      /*23  ...          */
        (seq_t)143616ULL,     /*24  ...          */
        (seq_t)1580544ULL,    /*25  Baby Monster (B) */
        (seq_t)424488960ULL   /*26  Monster (M)   */
    };

    if (n == 0 || n > 26) {
        return (seq_t)0;
    }
    return div_counts[n - 1];
}
